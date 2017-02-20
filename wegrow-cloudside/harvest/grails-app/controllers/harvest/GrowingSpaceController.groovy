package harvest

import grails.plugin.springsecurity.SpringSecurityUtils
import grails.plugin.springsecurity.annotation.Secured

import static org.springframework.http.HttpStatus.*
import grails.transaction.Transactional

@Transactional(readOnly = true)
@Secured("hasRole('ROLE_USER')")
class GrowingSpaceController {
    def springSecurityService
    def index() {
        def currentUser = springSecurityService?.currentUser

        if (currentUser.growingSpace) {
            redirect action: "edit", id: currentUser.growingSpace.id
        } else {
            redirect action: "create"
        }
    }

    def create() {}

}
