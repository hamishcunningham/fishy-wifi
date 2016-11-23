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
        def growingSpace = springSecurityService?.currentUser?.growingSpace
        if (SpringSecurityUtils.ifAllGranted("ROLE_ADMIN")) {
            respond Area.findAll(params), model:[areaCount: Area.count()]
        } else if (growingSpace != null) {
            params.max = Math.min(max ?: 10, 100)
            respond Area.findAllBySpace(growingSpace, params), model:[areaCount: Area.count()]
        }
    }

    def show(Area area) {
        respond area
    }

    def create() {
        def growingSpace = springSecurityService?.currentUser?.growingSpace

        if (growingSpace != null) {
            respond([area: new Area(params),
                    areaList: Area.findAllBySpace(growingSpace), areaCount: Area.count()])
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

        area.save flush:true

        request.withFormat {
            form multipartForm {
                flash.message = message(code: 'default.created.message',
                        args: [message(code: 'area.label', default: 'Area'), area.toString()])
                redirect action: "create"
            }
            '*' { respond area, [status: CREATED] }
        }
    }

    def edit(Area area) {
        respond area
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

        area.save flush:true

        request.withFormat {
            form multipartForm {
                flash.message = message(code: 'default.updated.message', args: [message(code: 'area.label', default: 'Area'), area.id])
                redirect area
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
