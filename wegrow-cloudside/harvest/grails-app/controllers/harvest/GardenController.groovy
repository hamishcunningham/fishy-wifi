package harvest

import grails.plugin.springsecurity.annotation.Secured
import grails.transaction.Transactional

import static org.springframework.http.HttpStatus.CREATED


/**
 * Created by dominic on 14/11/2016.
 */
@Secured("hasRole('ROLE_USER')")
class GardenController {
    static scaffold = Garden
}
