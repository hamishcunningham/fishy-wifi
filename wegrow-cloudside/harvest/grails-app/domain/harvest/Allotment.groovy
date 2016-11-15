package harvest

class Allotment extends GrowingSpace {
    String siteName
    Integer visitsPerWeek


    static constraints = {
        user(display: false)
        siteName()
        visitsPerWeek()
        area()
        unit()
        yearsGrowing()
        isOrganic()
        submittingAllData()
        consentGiven()
        electronicSignature()
    }
}
