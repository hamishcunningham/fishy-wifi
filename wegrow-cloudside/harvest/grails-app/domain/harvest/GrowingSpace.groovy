package harvest

import grails.util.GrailsNameUtils


class GrowingSpace {
    Integer yearsGrowing
    Integer monthsGrowing
    Boolean submittingAllData
    Boolean isOrganic
    Boolean consentGiven
    String electronicSignature

    Double areaMeters
    private Double area;
    private AreaUnit unit;

    def springSecurityService
    static belongsTo = [user:User]
    static hasMany = [harvests: Harvest, areas: Area]

    static transients = ['area', 'unit', 'springSecurityService', 'typeLabel']

    def getTypeLabel() {
        GrailsNameUtils.getPropertyName(this.class)
    }

    static constraints = {
        typeLabel display:false, bindable:false
        user nullable: true, display: false
        harvests display: false
        areaMeters display: false
        areas display: false
        area bindable: true, display: true
        unit display: false, bindable: true
    }

    def afterInsert() {
        if (user == null) {
            user = springSecurityService.currentUser
            user.growingSpace = this
            user.save()
        }
    }

    Double getArea() {
        User currentUser = springSecurityService.currentUser
        if (this.area != null) {
            return this.area
        } else if (areaMeters != null) {
            return getUnit().denormalise(areaMeters)
        } else {
            return areaMeters
        }
    }

    AreaUnit getUnit() {
        if (this.unit != null) {
            return this.unit
        } else if (springSecurityService.currentUser != null) {
            return springSecurityService.currentUser.preferredAreaUnit
        } else {
            return AreaUnit.getDefault()
        }
    }

    def setArea(area) {
        this.area = area
        if (area != null && unit != null) {
            areaMeters = unit.normalise(area)
        }
    }

    def setUnit(unit) {
        this.unit = unit
        if (area != null && unit != null) {
            areaMeters = unit.normalise(area)
        }
    }

}
