package harvest

class Aquaponics extends GrowingSpace {
    static constraints = {
        user(display: false)
        area()
        unit()
        yearsGrowing()
        isOrganic()
        submittingAllData()
        consentGiven  validator: { value, object ->
            value == true
        }
        electronicSignature()
    }
}
