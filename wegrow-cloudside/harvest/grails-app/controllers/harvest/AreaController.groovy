package harvest

import grails.plugin.springsecurity.SpringSecurityService
import grails.plugin.springsecurity.SpringSecurityUtils
import grails.plugin.springsecurity.annotation.Secured

import static org.springframework.http.HttpStatus.*
import grails.transaction.Transactional

@Secured("hasRole('ROLE_USER')")
@Transactional(readOnly = true)
class AreaController {

    static scaffold = Area

    static allowedMethods = [save: "POST", update: "PUT", delete: "DELETE"]

    SpringSecurityService springSecurityService;

    def index(Integer max) {
        def currentUser = springSecurityService?.currentUser

        if (SpringSecurityUtils.ifAllGranted("ROLE_ADMIN")) {
            respond Area.findAll(), model:[areaCount: Area.count()]
        } else {
            redirect controller: "harvest"
        }

    }

    def show(Area area) {
        if (area != null) {
            redirect action: "edit", id: area.id
        } else {
            notFound()
            return
        }
    }

    def create(params) {
        def currentUser = springSecurityService?.currentUser
        def growingSpace = currentUser?.growingSpace

        if (growingSpace != null) {
            respond([
                    area: new Area(),
                    areaList: Area.visibleAreas(currentUser).findAll(), areaCount: Area.count()])
        } else {
            redirect resource: "growingSpace", action: "create"
        }
    }

    @Transactional
    def save(Area area) {
        if (area == null) {
            transactionStatus.setRollbackOnly()
            notFound()
            return
        }

        if (area.hasErrors()) {
            transactionStatus.setRollbackOnly()
            respond area.errors, view:'create'
            return
        }

        if (!area.canEdit()) {
            transactionStatus.setRollbackOnly()
            notFound()
            return
        }

        area.finished = false

        area.save flush:true

        request.withFormat {
            form multipartForm {
                flash.message = message(code: 'default.created.message',
                        args: [message(code: 'area.label', default: 'Area'), area.toString()])
                redirect controller: "harvest", action: "index"
            }
            '*' { respond area, [status: CREATED] }
        }
    }

    def edit(Area area) {
        if (area.canEdit()) {
            respond area
        } else {
            notFound()
        }
    }

    @Transactional
    def update(Area area) {
        if (area == null) {
            transactionStatus.setRollbackOnly()
            notFound()
            return
        }

        if (area.hasErrors()) {
            transactionStatus.setRollbackOnly()
            respond area.errors, view:'edit'
            return
        }

        if (!area.canEdit()) {
            transactionStatus.setRollbackOnly()
            notFound()
            return
        }

        area.save flush:true

        request.withFormat {
            form multipartForm {
                flash.message = message(code: 'default.updated.message', args: [message(code: 'area.label', default: 'Area'), area.id])
                redirect action: "create"
            }
            '*'{ respond area, [status: OK] }
        }
    }

    @Transactional
    def delete(Area area) {

        if (area == null) {
            transactionStatus.setRollbackOnly()
            notFound()
            return
        }

        if (!area.canEdit()) {
            transactionStatus.setRollbackOnly()
            notFound()
            return
        }

        area.delete flush:true

        request.withFormat {
            form multipartForm {
                flash.message = message(code: 'default.deleted.message', args: [message(code: 'area.label', default: 'Area'), area.id])
                redirect action:"index", method:"GET"
            }
            '*'{ render status: NO_CONTENT }
        }
    }

    protected void notFound() {
        request.withFormat {
            form multipartForm {
                flash.message = message(code: 'default.not.found.message', args: [message(code: 'area.label', default: 'Area'), params.id])
                redirect action: "index", method: "GET"
            }
            '*'{ render status: NOT_FOUND }
        }
    }
}
