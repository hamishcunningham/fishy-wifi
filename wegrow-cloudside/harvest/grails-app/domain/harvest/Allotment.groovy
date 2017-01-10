package harvest

class Allotment extends GrowingSpace {
    String siteName
    Integer visitsPerWeek
    String address

    static constraints = {
        user(display: false)
        siteName()
        address()
        visitsPerWeek()
        area()
        unit()
        yearsGrowing()
        isOrganic()
        submittingAllData()
        consentGiven validator: { value, object ->
            value == true
        }
        electronicSignature()
    }
}
