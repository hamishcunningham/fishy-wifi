package harvest


class GrowingSpace {
    Integer yearsGrowing
    Boolean submittingAllData
    Boolean isOrganic
    Boolean consentGiven
    String electronicSignature

    Integer area
    AreaUnit unit
    static belongsTo = [user:User]

    static constraints = {
        user nullable: true, display: false
    }
}
