package harvest

import grails.plugin.springsecurity.annotation.Secured
import grails.transaction.Transactional

import static org.springframework.http.HttpStatus.CREATED


/**
 * Created by dominic on 14/11/2016.
 */
@Secured("hasRole('ROLE_USER')")
class AllotmentController {
    static scaffold = Allotment

    def springSecurityService
    @Transactional
    def save(Allotment allotment) {
        if (allotment == null) {
            transactionStatus.setRollbackOnly()
            notFound()
            return
        }

        if (allotment.hasErrors()) {
            transactionStatus.setRollbackOnly()
            respond allotment.errors, view:'create'
            return
        }
        springSecurityService.currentUser.growingSpace = allotment
        springSecurityService.currentUser.save flush:true
        allotment.save flush:true


        request.withFormat {
            form multipartForm {
                flash.message = message(code: 'default.created.message', args: [message(code: 'allotment.label', default: 'Allotment'), allotment.id])
                redirect allotment
            }
            '*' { respond allotment, [status: CREATED] }
        }
    }
}
