package harvest

class OtherGrowingSpace extends GrowingSpace {
    String type

    def getTypeLabel() {
        this.type
    }


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
