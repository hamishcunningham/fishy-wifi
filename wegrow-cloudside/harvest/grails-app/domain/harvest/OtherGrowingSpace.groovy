package harvest

class OtherGrowingSpace extends GrowingSpace {
    String type

    static constraints = {
        type()
        area()
        unit()
        yearsGrowing()
        isOrganic()
        submittingAllData()
        consentGiven()
        electronicSignature()

    }
}
