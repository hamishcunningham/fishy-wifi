package harvest
import grails.plugin.formfields.FormFieldsTagLib
import grails.core.GrailsDomainClass
import grails.core.GrailsDomainClassProperty
import org.grails.validation.DomainClassPropertyComparator

import static grails.util.GrailsClassUtils.getStaticPropertyValue


class FormFieldsHackTagLib extends FormFieldsTagLib  {
    //static encodeAsForTags = [tagName: [taglib:'html'], otherTagName: [taglib:'none']]
    FormFieldsTagLib formFieldsTagLib
    private List<GrailsDomainClassProperty> resolveAllProperties(GrailsDomainClass domainClass, attrs) {
        def properties

        if(attrs.order) {
            if(attrs.except) {
                throwTagError('The [except] and [order] attributes may not be used together.')
            }
            def orderBy = attrs.order?.tokenize(',')*.trim() ?: []
            properties = orderBy.collect { propertyName -> domainClass.getProperty(propertyName) }
        } else {
            properties = domainClass.properties as List
            def blacklist = attrs.except?.tokenize(',')*.trim() ?: []
            blacklist << 'dateCreated' << 'lastUpdated'
            def scaffoldProp = getStaticPropertyValue(domainClass.clazz, 'scaffold')
            if (scaffoldProp) {
                blacklist.addAll(scaffoldProp.exclude)
            }
            properties.removeAll { it.name in blacklist }
            properties.removeAll { !it.domainClass.constrainedProperties[it.name]?.display }
            properties.removeAll { it.derived }

            Collections.sort(properties, new org.grails.validation.DomainClassPropertyComparator(domainClass))
        }
        properties
    }


    /**
     * @attr bean REQUIRED Name of the source bean in the GSP model.
     * @attr except A comma-separated list of properties to exclude from
     * the generated list of input fields.
     * @attr prefix Prefix to add to input element names.
     */
    def allWTransients = { attrs ->
        if (!attrs.bean) throwTagError("Tag [all] is missing required attribute [bean]")
        def bean = resolveBean(attrs.bean)
        def domainClass = resolveDomainClass(bean)
        def prefix = resolvePrefix(attrs.prefix)

        if (domainClass) {
            for (property in resolveAllProperties(domainClass, attrs)) {
                out << f.field(bean: bean, property: property.name, prefix: prefix)
            }
        } else {
            throwTagError('Tag [all] currently only supports domain types')
        }
    }

}
