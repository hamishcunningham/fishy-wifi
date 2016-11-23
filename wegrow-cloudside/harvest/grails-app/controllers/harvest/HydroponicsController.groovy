package harvest

import grails.plugin.springsecurity.annotation.Secured

/**
 * Created by dominic on 14/11/2016.
 */
@Secured("hasRole('ROLE_USER')")
class HydroponicsController {
    static scaffold = Hydroponics
}
