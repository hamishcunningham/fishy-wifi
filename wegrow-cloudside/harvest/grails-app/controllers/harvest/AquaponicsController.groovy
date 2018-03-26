package harvest

import grails.plugin.springsecurity.annotation.Secured

@Secured("hasRole('ROLE_USER')")
class AquaponicsController {
    static scaffold = Aquaponics
}
