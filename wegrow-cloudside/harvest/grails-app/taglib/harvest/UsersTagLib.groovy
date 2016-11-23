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



}