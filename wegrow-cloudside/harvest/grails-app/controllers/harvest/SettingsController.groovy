package harvest

import grails.plugin.springsecurity.annotation.Secured

import static org.springframework.http.HttpStatus.*
import grails.transaction.Transactional

@Secured("hasRole('ROLE_ADMIN')")
@Transactional(readOnly = true)
class SettingsController {

    static allowedMethods = [update: "PUT"]

    def index(Integer max) {
        redirect action:"edit"
    }


    def edit() {
        respond Settings.first()
    }

    @Transactional
    def update(Settings settings) {
        if (settings == null) {
            transactionStatus.setRollbackOnly()
            notFound()
            return
        }

        if (settings.hasErrors()) {
            transactionStatus.setRollbackOnly()
            respond settings.errors, view:'edit'
            return
        }

        settings.save flush:true

        request.withFormat {
            form multipartForm {
                flash.message = message(code: 'default.updated.message', args: [message(code: 'settings.label', default: 'Settings'), settings.id])
                redirect action:"edit"
            }
            '*'{ respond action:"edit", [status: OK] }
        }
    }


}
