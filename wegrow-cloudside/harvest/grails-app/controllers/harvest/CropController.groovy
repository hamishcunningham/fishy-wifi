package harvest

import grails.plugin.springsecurity.annotation.Secured
import grails.plugins.export.ExportService

import static org.springframework.http.HttpStatus.*
import grails.transaction.Transactional

@Secured("hasRole('ROLE_ADMIN')")
@Transactional(readOnly = true)
class CropController {

    static scaffold = Crop
    ExportService exportService

    def index(Integer max) {
        if (params?.f) {

            def fields = ["type", "isTree", "greenhouseable", "areas", "harvests", "av_yield"]
            def data = Crop.list().collect { c ->
                [
                        type: c.type,
                        isTree: c.isTree,
                        greenhouseable: c.isGreenhouseable,
                        areas: c.areas.size(),
                        harvests: c.harvests.size(),
                        av_yield: c.yield,
                        id: c.id
                ]
            }
            response.contentType = grailsApplication.config.grails.mime.types[params.f]
            response.setHeader("Content-disposition", "attachment; filename=harvests.${params.extension}")
            exportService.export(params.f, response.outputStream, data, fields, [:],[:],[:])
        } else {
            params.max = Math.min(max ?: 10, 100)
            respond Crop.list(params), model:[cropCount: Crop.count()]
        }
    }

    @Secured("hasRole('ROLE_USER')")
    def show(Crop crop) {
        respond crop
    }
}
