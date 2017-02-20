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

    def index(Integer offset, Integer max) {
        if (params?.f) {
            def harvests = Harvest.visibleHarvests(springSecurityService.currentUser)

            def data = harvests.list().collect { Harvest harvest ->
                [
                        email: harvest.area.space.user.email,
                        id: harvest.id,
                        area_id: harvest.area.id,
                        area_m2: !harvest.area.crop.isTree ? // Use the radius to get the area for trees.
                                harvest.area.areaMeters : (harvest.area.canopyRadius ** 2) * Math.PI,
                        crop: harvest.area.crop,
                        logged_at: harvest.dateCreated,
                        yield_m2: harvest.yield,
                        weight_g: harvest.weightGrammes,
                        type: harvest.area.space.typeLabel,
                        organic: harvest.area.space.isOrganic,
                        all_data: harvest.area.space.submittingAllData,
                        growing_space: harvest.area.space.typeLabel,
                        radius: harvest.area.canopyRadius,
                        in_greenhouse: harvest.area.inGreenhouse
                ]
            }
            def fields = ["id",
                          "logged_at",
                          "growing_space",
                          "email",
                          "yield_m2",
                          "area_id",
                          "area_m2",
                          "radius",
                          "weight_g",
                          "crop",
                          "type",
                          "organic",
                          "in_greenhouse",
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
            // Group harvests by area and then display.
            max = max?:10
            offset = offset != null? offset :0

            def visible = Area.visibleAreas(springSecurityService.currentUser)
            respond areaList: visible.list(max: max, offset: offset), areaCount: visible.count()
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

    def create(Integer areaId) {

        def allowedAreas = Area.selectableAreas(springSecurityService.currentUser).findAll()
        if (allowedAreas.isEmpty()) {
            flash.message = g.message code: "harvest.area.noCropRegistered", default: "No crop registered."
            flash.messageType = "alert"
            return redirect(controller: "area", action: "create")
        }

        def selectedArea = Area.selectableAreas(springSecurityService.currentUser).findById(areaId)
        respond([harvest: new Harvest(params), allowedAreas: allowedAreas, selectedArea: selectedArea])
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

        harvest.area.finished = (cropFinished == true)
        harvest.area.save()


        request.withFormat {
            form multipartForm {
                flash.message = message(code:"harvest.logged");
                redirect action: "index"
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
