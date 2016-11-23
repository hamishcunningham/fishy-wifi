package harvest

import grails.plugin.springsecurity.SpringSecurityService
import grails.plugin.springsecurity.annotation.Secured

import static org.springframework.http.HttpStatus.*
import grails.transaction.Transactional

@Transactional(readOnly = true)
@Secured("hasRole('ROLE_USER')")
class HarvestController {

    static allowedMethods = [save: "POST", update: "PUT", delete: "DELETE"]
  SpringSecurityService springSecurityService;

    def index(Integer max) {
      if (SpringSecurityUtils.ifAllGranted("ROLE_ADMIN")) {
        respond Harvest.findAll()
      } else {
        def growingSpace = springSecurityService?.currentUser?.growingSpace

        if (growingSpace != null) {
          def harvests = Harvest.where {
            area.space == growingSpace
          }
          params.max = Math.min(max ?: 10, 100)
          respond harvests.findAll(params)
        }
      }
    }

    def show(Harvest harvest) {
      if (harvest.area.space == springSecurityService.currentUser.growingSpace) {
        respond harvest
      } else {
        notFound()
      }
    }

    def create() {
        respond new Harvest(params)
    }

    @Transactional
    def save(Harvest harvest) {
        if (harvest == null) {
            transactionStatus.setRollbackOnly()
            notFound()
            return
        }

        if (harvest.hasErrors()) {
            transactionStatus.setRollbackOnly()
            respond harvest.errors, view:'create'
            return
        }

        harvest.save flush:true

        request.withFormat {
            form multipartForm {
                flash.message = message(code: 'default.created.message', args: [message(code: 'harvest.label', default: 'Harvest'), harvest.id])
                redirect harvest
            }
            '*' { respond harvest, [status: CREATED] }
        }
    }

    def edit(Harvest harvest) {
        respond harvest
    }

    @Transactional
    def update(Harvest harvest) {
        if (harvest == null) {
            transactionStatus.setRollbackOnly()
            notFound()
            return
        }

        if (harvest.hasErrors()) {
            transactionStatus.setRollbackOnly()
            respond harvest.errors, view:'edit'
            return
        }

        harvest.save flush:true

        request.withFormat {
            form multipartForm {
                flash.message = message(code: 'default.updated.message', args: [message(code: 'harvest.label', default: 'Harvest'), harvest.id])
                redirect harvest
            }
            '*'{ respond harvest, [status: OK] }
        }
    }

    @Transactional
    def delete(Harvest harvest) {

        if (harvest == null) {
            transactionStatus.setRollbackOnly()
            notFound()
            return
        }

        harvest.delete flush:true

        request.withFormat {
            form multipartForm {
                flash.message = message(code: 'default.deleted.message', args: [message(code: 'harvest.label', default: 'Harvest'), harvest.id])
                redirect action:"index", method:"GET"
            }
            '*'{ render status: NO_CONTENT }
        }
    }

    protected void notFound() {
        request.withFormat {
            form multipartForm {
                flash.message = message(code: 'default.not.found.message', args: [message(code: 'harvest.label', default: 'Harvest'), params.id])
                redirect action: "index", method: "GET"
            }
            '*'{ render status: NOT_FOUND }
        }
    }
}
