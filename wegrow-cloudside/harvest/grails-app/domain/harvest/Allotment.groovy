package harvest

class Allotment extends GrowingSpace {
    String siteName
    Integer visitsPerWeek
    String address

    static constraints = {
        user(display: false)
        siteName()
        visitsPerWeek()
        area()
        unit()
        yearsGrowing()
        monthsGrowing()
        isOrganic()
        submittingAllData()
        consentGiven validator: { value, object ->
            value == true
        }
        electronicSignature()
    }
}
