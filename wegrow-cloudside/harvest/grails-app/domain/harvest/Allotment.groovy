package harvest

class Allotment extends GrowingSpace {
    String siteName
    Integer visitsPerWeek
    String address

    @Override
    String getLocationString() {
        "${address}, ${siteName}"
    }


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
