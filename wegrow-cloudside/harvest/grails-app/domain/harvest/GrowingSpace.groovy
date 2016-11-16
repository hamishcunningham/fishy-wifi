package harvest


class GrowingSpace {
    Integer yearsGrowing
    Boolean submittingAllData
    Boolean isOrganic
    Boolean consentGiven
    String electronicSignature

    Double areaMeters
    Double area
    AreaUnit unit

    transient springSecurityService
    static belongsTo = [user:User]
    static hasMany = [harvests: Harvest, areas: Area]

    static transients = ['area', 'unit', 'springSecurityService']


    static constraints = {
        user nullable: true, display: false
        harvests display: false
        areaMeters display: false
        areas display: false

        area bindable: true, display: true
        unit display: false
    }

    def afterInsert() {
        if (user == null) {
            user = springSecurityService.currentUser
            user.growingSpace = this
            user.save()
        }
    }

    def beforeValidate() {
        if (area != null) {
            areaMeters = unit.normalise(area)
        }
    }
}
