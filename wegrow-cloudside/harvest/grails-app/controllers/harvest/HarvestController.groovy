package harvest

import grails.plugin.springsecurity.SpringSecurityService
import grails.plugin.springsecurity.SpringSecurityUtils
import grails.plugin.springsecurity.annotation.Secured

import static org.springframework.http.HttpStatus.*
import grails.transaction.Transactional

@Transactional(readOnly = true)
@Secured("hasRole('ROLE_USER')")
class HarvestController {
    static allowedMethods = [save: "POST", update: "PUT", delete: "DELETE"]
    SpringSecurityService springSecurityService;
    def exportService

    def index(Integer max) {
        def harvests = Harvest.visibleHarvests(springSecurityService.currentUser)

        if (params?.f) {
            def data = harvests.findAll().collect { harvest ->
                [
                        id: harvest.id,
                        area_id: harvest.area.id,
                        area_m2: harvest.area.area,
                        crop: harvest.area.crop,
                        weight_g: harvest.weight,
                        type: harvest.area.space.typeLabel,
                        organic: harvest.area.space.isOrganic,
                        all_data: harvest.area.space.submittingAllData,
                        growingSpace: harvest.area.space.growingSpace

                ]
            }
            def fields = ["id",
                          "area_id",
                          "area_m2",
                          "weight_g",
                          "crop",
                          "type",
                          "organic",
                          "all_data"]

            if (data.isEmpty()) {
                flash.message = message(code: 'harvest.export.failed', default: "Export failed - no data available.")
                respond harvests.findAll()
            } else {
                response.contentType = grailsApplication.config.grails.mime.types[params.f]
                response.setHeader("Content-disposition", "attachment; filename=harvests.${params.extension}")
                exportService.export(params.f, response.outputStream, data, fields, [:],[:],[:])
            }
        } else {
            respond harvests.findAll()
        }
    }

    def show(Harvest harvest) {
      if (harvest.area.space == springSecurityService.currentUser.growingSpace ||
        SpringSecurityUtils.ifAllGranted("ROLE_ADMIN"))  {
        respond harvest
      } else {
        notFound()
      }
    }

    def create() {
        def allowedAreas = Area.selectableAreas(springSecurityService.currentUser).findAll()
        if (allowedAreas.isEmpty()) {
            flash.message = g.message code: "harvest.area.noCropRegistered", default: "No crop registered."
            flash.messageType = "alert"
            return redirect(controller: "area", action: "create")
        }
        respond([harvest: new Harvest(params), allowedAreas: allowedAreas])
    }

    @Transactional
    def save(Harvest harvest, Boolean cropFinished) {
        if (harvest == null) {
            transactionStatus.setRollbackOnly()
            notFound()
            return
        }

        if (!harvest.canEdit()) {
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

        harvest.area.finished = cropFinished
        harvest.area.save()


        request.withFormat {
            form multipartForm {
                flash.message = message(code: 'default.created.message', args: [message(code: 'harvest.label', default: 'Harvest'), harvest.id])
                redirect action: "create"
            }
            '*' { respond harvest, [status: CREATED] }
        }
    }

    def edit(Harvest harvest) {
        if (!harvest.canEdit()) {
            notFound()
            return
        }

        respond harvest
    }

    @Transactional
    def update(Harvest harvest) {
        if (harvest == null) {
            transactionStatus.setRollbackOnly()
            notFound()
            return
        }

        if (!harvest.canEdit()) {
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

        if (!harvest.canEdit()) {
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
