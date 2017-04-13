package harvest

import grails.plugin.springsecurity.annotation.Secured

import static org.springframework.http.HttpStatus.*
import grails.transaction.Transactional

@Transactional(readOnly = true)
@Secured("hasRole('ROLE_USER')")
class GardenController {
    static scaffold = Garden



    @Transactional
    def save(Garden garden) {
        garden.areaMeters = 10
        garden.validate()
        if (garden == null) {
            transactionStatus.setRollbackOnly()
            notFound()
            return
        }

        if (garden.hasErrors()) {
            transactionStatus.setRollbackOnly()
            respond garden.errors, view:'create'
            return
        }

        garden.save flush:true

        request.withFormat {
            form multipartForm {
                flash.message = message(code: 'default.created.message', args: [message(code: 'garden.label', default: 'Garden'), garden.id])
                redirect garden
            }
            '*' { respond garden, [status: CREATED] }
        }
    }

}
