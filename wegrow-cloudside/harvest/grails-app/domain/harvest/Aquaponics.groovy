package harvest

class Aquaponics extends GrowingSpace {
    
    Double volumeM3
    private Double volume
    private VolumeUnit volumeUnit
    
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
