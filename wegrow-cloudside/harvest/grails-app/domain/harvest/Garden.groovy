package harvest

class Garden extends GrowingSpace {

    static constraints = {
        area bindable: true, display: true
        yearsGrowing()
        isOrganic()
        submittingAllData()
        consentGiven()
        electronicSignature()
    }
}
