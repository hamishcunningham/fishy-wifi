package harvest

class OtherGrowingSpace extends PostcodeGrowingSpace {
    String type

    def getTypeLabel() {
        this.type
    }
    
    static constraints = {
        type()
        postCode()
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
