package harvest

import grails.plugin.springsecurity.SpringSecurityService

class UsersTagLib {
    SpringSecurityService springSecurityService

    def unitRadio = { attrs, body ->
        def radioAttrs = ["value": attrs.unit.toString(), "name": attrs.name]
        if (attrs.unit == springSecurityService.currentUser[attrs.preference]) {
            radioAttrs["checked"] = "checked"
        }
        out << g.radio(radioAttrs, {
            out << " " << attrs.unit.name
        })
    }

    def formatNumberUnit = { attrs, body ->
        def objectType = attrs.remove("type")
        attrs.type = "number"
        def value = attrs.remove("number")

        if (objectType.equalsIgnoreCase("WEIGHT")) {
            def unit = springSecurityService.currentUser.preferredWeightUnit

            def formattedValue = value ? unit.denormalise(value) : value
            attrs.number = formattedValue
            out << g.formatNumber(attrs) << " " << unit.name
        } else if (objectType.equalsIgnoreCase("AREA")) {
            def unit = springSecurityService.currentUser.preferredAreaUnit
            def formattedValue = value ? unit.denormalise(value) : value

            attrs.number = formattedValue
            out << g.formatNumber(attrs) << " " << unit.name
        } else if (objectType.equalsIgnoreCase("RADIUS")) {
            def unit = springSecurityService.currentUser.preferredLengthUnit

            def formattedValue = value ? unit.denormalise(value) : value
            attrs.number = formattedValue
            out << g.formatNumber(attrs) << " " << unit.name
        } else if (objectType.equalsIgnoreCase("YIELD")) {
            attrs.number = value / 1000
            out << g.formatNumber(attrs) << " kg/m²"
        }
    }


}