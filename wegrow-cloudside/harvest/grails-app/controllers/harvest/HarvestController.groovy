package harvest

import grails.plugin.springsecurity.SpringSecurityService
import grails.plugin.springsecurity.SpringSecurityUtils
import grails.plugin.springsecurity.annotation.Secured

import static org.springframework.http.HttpStatus.*
import grails.transaction.Transactional

import grails.rest.*

@Transactional(readOnly = true)
@Secured("hasRole('ROLE_USER')")
class HarvestController {
    static allowedMethods = [save: "POST", update: "PUT", delete: "DELETE"]
    
    SpringSecurityService springSecurityService;
    def exportService
    
    def index(Integer offset, Integer max, Boolean activeOnly) {
        if (params?.f) {
            def harvests = Harvest.visibleHarvests(springSecurityService.currentUser)

/* TODO
Need all the data to be consilidated into one spreadsheet when we download it.
Obviously there will be a lot of replication in the spreadsheet because users
will have multiple harvests but the way things stand we can't interpret the
data properly.  This is because each person that uses the site gives an email
address but if they are reporting yields from their garden and from their
allotment for example ((but could be other growing space) they have multiple
usernames associated with the same email address.  At present when we download
the harvest data we only get the email address and not the username and so we
can't tell where that person is growing their crop.  In addition, in the sign
up data people provide the information below to us, but we don't get it when
we download the data:

Type of Growing Space (Allotment / Garden / Other)
Total Growing Space (for Allotments or Other)
Name of Allotment Site (Allotment only)
Number of Visits to Site per Week (Allotment only)
Description of the growing space (Other only)
and with the harvest download data:

The harvest data doesn't show whether the "This was the last harvest of the
year from this crop." (a box ticked, or not, when each individual harvest is
submitted). 


TODOs 2

Time growing in registered growing space
Allotment site address
Postcode of garden growing space
Postcode of other growing space
*/
            def data = harvests.list().collect { Harvest harvest ->
                [
                        email: harvest.area.space.user.email,
                        username: harvest.area.space.user.username,
                        space_type: harvest.area.space.class.name,
                        space_area: harvest.area.space.areaMeters,
                        // space_address: harvest.area.space?.address ?: "no address",
                        space_location: harvest.area.space.instanceOf(PostcodeGrowingSpace) ? harvest.area.space.postCode : harvest.area.space.getLocationString(),
                        space_timegrowing: harvest.area.space.yearsGrowing,
                        id: harvest.id,
                        area_label: harvest.area.name,

                        area_id: harvest.area.id,
                        area_m2: !harvest.area.crop.isTree ? // Use the radius to get the area for trees.
                                harvest.area.areaMeters : (harvest.area.canopyRadiusMeters ** 2) * Math.PI,
                        crop: harvest.area.crop,
                        variety: harvest.area.variety,
                        logged_at: harvest.dateHarvested,
                        yield_m2: harvest.yield,
                        weight_g: harvest.weightGrammes,
                        type: harvest.area.space.typeLabel,
                        organic: harvest.area.space.isOrganic ? "yes" : "no",
                        all_data: harvest.area.space.submittingAllData ? "yes" : "no",
                        growing_space: harvest.area.space.typeLabel ? "yes" : "no",
                        allotment_name: harvest.area.space.instanceOf(Allotment)  ? harvest.area.space.siteName : "",
                        allotment_visits_per_week: harvest.area.space.instanceOf(Allotment)  ? harvest.area.space.visitsPerWeek : "",
                        last_harvest: harvest.area.finished ? "yes" : "no", 
                        growing_space_m2: harvest.area.space.areaMeters,
                        radius: harvest.area.canopyRadiusMeters,
                        in_greenhouse: harvest.area.inGreenhouse ? "yes" : "no"
                ]
            }
            def fields = ["id",
                          "username",
                          "logged_at",
                          "last_harvest",
                          "growing_space",
                          "space_type",
                          "space_location",
                          "space_timegrowing",
                          "space_area",
                          "email",
                          "yield_m2",
                          "area_label",
                          "area_id",
                          "area_m2",
                          "radius",
                          "weight_g",
                          "crop",
                          "variety",
                          "type",
                          "growing_space_m2",
                          "allotment_name",
                          "allotment_visits_per_week",
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
            activeOnly = activeOnly?:false;

            def visible = Area.visibleAreas(springSecurityService.currentUser, activeOnly)

			//this sorts by crop type, but the sort order within crop type is differnet for all vs active harvests
            respond areaList: visible.list(max: max, offset: offset) {
              order "crop.type"
              order "id"
            }, areaCount: visible.count()
        }
    }

    def show(Harvest harvest) {
        if (harvest.canEdit())  {
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
    def update(Harvest harvest, Boolean cropFinished) {
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
        harvest.area.finished = (cropFinished == true)
        harvest.area.save()

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
