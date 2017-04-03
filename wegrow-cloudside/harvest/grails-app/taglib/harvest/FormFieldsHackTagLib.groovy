package harvest
import grails.plugin.formfields.FormFieldsTagLib
import grails.core.GrailsDomainClass
import grails.core.GrailsDomainClassProperty
import grails.plugin.springsecurity.ui.CommandObject
import groovy.transform.CompileStatic
import org.grails.plugins.web.GrailsTagDateHelper
import org.grails.validation.DomainClassPropertyComparator
import org.springframework.web.servlet.support.RequestContextUtils
import org.springframework.web.servlet.support.RequestDataValueProcessor

import java.text.DateFormat
import java.text.DateFormatSymbols

import static grails.util.GrailsClassUtils.getStaticPropertyValue


class FormFieldsHackTagLib extends FormFieldsTagLib  {
    //static encodeAsForTags = [tagName: [taglib:'html'], otherTagName: [taglib:'none']]
    FormFieldsTagLib formFieldsTagLib
    RequestDataValueProcessor requestDataValueProcessor

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
            properties.removeAll {
                !it.domainClass.constrainedProperties[it.name]?.display
            }
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
        if (!attrs.bean) throwTagError("Tag [allWTransients] is missing required attribute [bean]")
        def bean = resolveBean(attrs.bean)
        def domainClass = resolveDomainClass(bean)

        def prefix = resolvePrefix(attrs.prefix)

        if (domainClass) {
            for (property in resolveAllProperties(domainClass, attrs)) {
                out << f.field(bean: bean, property: property.name, prefix: prefix)
            }
        }  else {
            throwTagError('Tag [allWTransients] currently only supports domain types')
        }
    }
    def renderNoSelectionOptionImpl(out, noSelectionKey, noSelectionValue, value) {
        // If a label for the '--Please choose--' first item is supplied, write it out
        out << "<option value=\"${(noSelectionKey == null ? '' : noSelectionKey)}\"${noSelectionKey == value ? ' selected="selected"' : ''}>${noSelectionValue.encodeAsHTML()}</option>"
    }

    def eachProperty = { attrs, body ->
        def domainClass = attrs.domainClass
        def var = attrs.var ?: "property"
        def indexKey = attrs.status ?: "index"

        if (domainClass) {
            resolveAllProperties(domainClass, attrs).eachWithIndex { property, index ->
                out << body((var):property, (indexKey):index)
            }
        } else {
            throwTagError('Tag [eachProperty] currently only supports domain types')
        }

    }
    GrailsTagDateHelper grailsTagDateHelper

    /**
     * Some attributes can be defined as Boolean values, but the html specification
     * mandates the attribute must have the same value as its name. For example,
     * disabled, readonly and checked.
     */
    @CompileStatic
    private void booleanToAttribute(Map attrs, String attrName) {
        def attrValue = attrs.remove(attrName)
        if (attrValue instanceof CharSequence) {
            attrValue = attrValue.toString().trim()
        }
        // If the value is the same as the name or if it is a boolean value,
        // reintroduce the attribute to the map according to the w3c rules, so it is output later
        if ((attrValue instanceof Boolean && attrValue) ||
                (attrValue instanceof String && (((String)attrValue).equalsIgnoreCase("true") || ((String)attrValue).equalsIgnoreCase(attrName)))) {
            attrs.put(attrName, attrName)
        } else if (attrValue instanceof String && !((String)attrValue).equalsIgnoreCase("false")) {
            // If the value is not the string 'false', then we should just pass it on to
            // keep compatibility with existing code
            attrs.put(attrName, attrValue)
        }
    }

    private processFormFieldValueIfNecessary(name, value, type) {
        if (requestDataValueProcessor != null) {
            return requestDataValueProcessor.processFormFieldValue(request, name, "${value}", type)
        }
        return value
    }

    /**
     * A simple date picker that renders a date as selects.<br/>
     * e.g. &lt;g:datePicker name="myDate" value="${new Date()}" /&gt;
     *
     * @emptyTag
     *
     * @attr name REQUIRED The name of the date picker field set
     * @attr value The current value of the date picker; defaults to either the value specified by the default attribute or now if no default is set
     * @attr default A Date or parsable date string that will be used if there is no value
     * @attr precision The desired granularity of the date to be rendered
     * @attr noSelection A single-entry map detailing the key and value to use for the "no selection made" choice in the select box. If there is no current selection this will be shown as it is first in the list, and if submitted with this selected, the key that you provide will be submitted. Typically this will be blank.
     * @attr years A list or range of years to display, in the order specified. i.e. specify 2007..1900 for a reverse order list going back to 1900. If this attribute is not specified, a range of years from the current year - 100 to current year + 100 will be shown.
     * @attr relativeYears A range of int representing values relative to value. For example, a relativeYears of -2..7 and a value of today will render a list of 10 years starting with 2 years ago through 7 years in the future. This can be useful for things like credit card expiration dates or birthdates which should be bound relative to today.
     * @attr id the DOM element id
     * @attr disabled Makes the resulting inputs and selects to be disabled. Is treated as a Groovy Truth.
     * @attr readonly Makes the resulting inputs and selects to be made read only. Is treated as a Groovy Truth.
     */
    Closure datePickerWClass = { attrs ->
        def out = out // let x = x ?
        def xdefault = attrs['default']

        def klass = attrs.klass ?:"datePicker"
        if (xdefault == null) {
            xdefault = new Date()
        }
        else if (xdefault.toString() != 'none') {
            if (xdefault instanceof String) {
                xdefault = DateFormat.getInstance().parse(xdefault)

            }
            else if (!grailsTagDateHelper.supportsDatePicker(xdefault.class)) {
                throwTagError("Tag [datePicker] the default date is not a supported class")
            }
        }
        else {
            xdefault = null
        }
        def years = attrs.years
        def relativeYears = attrs.relativeYears
        if (years != null && relativeYears != null) {
            throwTagError 'Tag [datePicker] does not allow both the years and relativeYears attributes to be used together.'
        }

        if (relativeYears != null) {
            if (!(relativeYears instanceof IntRange)) {
                // allow for a syntax like relativeYears="[-2..5]".  The value there is a List containing an IntRage.
                if ((!(relativeYears instanceof List)) || (relativeYears.size() != 1) || (!(relativeYears[0] instanceof IntRange))) {
                    throwTagError 'The [datePicker] relativeYears attribute must be a range of int.'
                }
                relativeYears = relativeYears[0]
            }
        }
        def value = attrs.value
        if (value.toString() == 'none') {
            value = null
        }
        else if (!value) {
            value = xdefault
        }
        def name = attrs.name
        def id = attrs.id ?: name

        def noSelection = attrs.noSelection
        if (noSelection != null) {
            noSelection = noSelection.entrySet().iterator().next()
        }

        // make below final once GROOVY-8093 is fixed
        def PRECISION_RANKINGS = ["year": 0, "month": 10, "day": 20, "hour": 30, "minute": 40]
        def precision = (attrs.precision ? PRECISION_RANKINGS[attrs.precision] :
                (grailsApplication.config.grails.tags.datePicker.default.precision ?
                        PRECISION_RANKINGS["${grailsApplication.config.grails.tags.datePicker.default.precision}"] :
                        PRECISION_RANKINGS["minute"]))

        def day
        def month
        def year
        def hour
        def minute
        def dfs = new DateFormatSymbols(RequestContextUtils.getLocale(request))

        def c = null
        if (value instanceof Calendar) {
            c = value
        }
        else if (value != null) {
            c = grailsTagDateHelper.buildCalendar(value)
        }

        if (c != null) {
            day = c.get(GregorianCalendar.DAY_OF_MONTH)
            month = c.get(GregorianCalendar.MONTH)
            year = c.get(GregorianCalendar.YEAR)
            hour = c.get(GregorianCalendar.HOUR_OF_DAY)
            minute = c.get(GregorianCalendar.MINUTE)
        }

        if (years == null) {
            def tempyear
            if (year == null) {
                // If no year, we need to get current year to setup a default range... ugly
                def tempc = new GregorianCalendar()
                tempc.setTime(new Date())
                tempyear = tempc.get(GregorianCalendar.YEAR)
            }
            else {
                tempyear = year
            }
            if (relativeYears) {
                if (relativeYears.reverse) {
                    years = (tempyear + relativeYears.toInt)..(tempyear + relativeYears.fromInt)
                } else {
                    years = (tempyear + relativeYears.fromInt)..(tempyear + relativeYears.toInt)
                }
            } else {
                years = (tempyear + 100)..(tempyear - 100)
            }
        }

        booleanToAttribute(attrs, 'disabled')
        booleanToAttribute(attrs, 'readonly')

        // Change this hidden to use requestDataValueProcessor
        def dateStructValue = processFormFieldValueIfNecessary("${name}","date.struct","hidden")
        out.println "<input type=\"hidden\" name=\"${name}\" value=\"${dateStructValue}\" />"

        // create day select
        if (precision >= PRECISION_RANKINGS["day"]) {
            out.println "<select class=\"${klass}\" name=\"${name}_day\" id=\"${id}_day\" aria-labelledby=\"${name}\""
            if (attrs.disabled) {
                out << ' disabled="disabled"'
            }
            if (attrs.readonly) {
                out << ' readonly="readonly"'
            }
            out << '>'

            if (noSelection) {
                renderNoSelectionOptionImpl(out, noSelection.key, noSelection.value, '')
                out.println()
            }

            for (i in 1..31) {
                // Change this option to use requestDataValueProcessor
                def dayIndex = processFormFieldValueIfNecessary("${name}_day","${i}","option")
                out.println "<option value=\"${dayIndex}\"${i == day ? ' selected="selected"' : ''}>${i}</option>"
            }
            out.println '</select>'
        }

        // create month select
        if (precision >= PRECISION_RANKINGS["month"]) {
            out.println "<select class=\"${klass}\" name=\"${name}_month\" id=\"${id}_month\" aria-labelledby=\"${name}\""
            if (attrs.disabled) {
                out << ' disabled="disabled"'
            }
            if (attrs.readonly) {
                out << ' readonly="readonly"'
            }
            out << '>'

            if (noSelection) {
                renderNoSelectionOptionImpl(out, noSelection.key, noSelection.value, '')
                out.println()
            }

            dfs.months.eachWithIndex {m, i ->
                if (m) {
                    def monthIndex = i + 1
                    monthIndex = processFormFieldValueIfNecessary("${name}_month","${monthIndex}","option")
                    out.println "<option value=\"${monthIndex}\"${i == month ? ' selected="selected"' : ''}>$m</option>"
                }
            }
            out.println '</select>'
        }

        // create year select
        if (precision >= PRECISION_RANKINGS["year"]) {
            out.println "<select class=\"${klass}\" name=\"${name}_year\" id=\"${id}_year\" aria-labelledby=\"${name}\""
            if (attrs.disabled) {
                out << ' disabled="disabled"'
            }
            if (attrs.readonly) {
                out << ' readonly="readonly"'
            }
            out << '>'

            if (noSelection) {
                renderNoSelectionOptionImpl(out, noSelection.key, noSelection.value, '')
                out.println()
            }

            for (i in years) {
                // Change this year option to use requestDataValueProcessor
                def yearIndex  = processFormFieldValueIfNecessary("${name}_year","${i}","option")
                out.println "<option value=\"${yearIndex}\"${i == year ? ' selected="selected"' : ''}>${i}</option>"
            }
            out.println '</select>'
        }

        // do hour select
        if (precision >= PRECISION_RANKINGS["hour"]) {
            out.println "<select class=\"${klass}\" name=\"${name}_hour\" id=\"${id}_hour\" aria-labelledby=\"${name}\""
            if (attrs.disabled) {
                out << ' disabled="disabled"'
            }
            if (attrs.readonly) {
                out << ' readonly="readonly"'
            }
            out << '>'

            if (noSelection) {
                renderNoSelectionOptionImpl(out, noSelection.key, noSelection.value, '')
                out.println()
            }

            for (i in 0..23) {
                def h = '' + i
                if (i < 10) h = '0' + h
                // This option add hour to requestDataValueProcessor
                h  = processFormFieldValueIfNecessary("${name}_hour","${h}","option")
                out.println "<option value=\"${h}\"${i == hour ? ' selected="selected"' : ''}>$h</option>"
            }
            out.println '</select> :'

            // If we're rendering the hour, but not the minutes, then display the minutes as 00 in read-only format
            if (precision < PRECISION_RANKINGS["minute"]) {
                out.println '00'
            }
        }

        // do minute select
        if (precision >= PRECISION_RANKINGS["minute"]) {
            out.println "<select class=\"${klass}\" name=\"${name}_minute\" id=\"${id}_minute\" aria-labelledby=\"${name}\""
            if (attrs.disabled) {
                out << 'disabled="disabled"'
            }
            if (attrs.readonly) {
                out << 'readonly="readonly"'
            }
            out << '>'

            if (noSelection) {
                renderNoSelectionOptionImpl(out, noSelection.key, noSelection.value, '')
                out.println()
            }

            for (i in 0..59) {
                def m = '' + i
                if (i < 10) m = '0' + m
                m  = processFormFieldValueIfNecessary("${name}_minute","${m}","option")
                out.println "<option value=\"${m}\"${i == minute ? ' selected="selected"' : ''}>$m</option>"
            }
            out.println '</select>'
        }
    }
}
