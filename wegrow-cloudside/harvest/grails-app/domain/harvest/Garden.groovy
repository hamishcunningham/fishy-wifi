package harvest

class Garden extends PostcodeGrowingSpace {


	static constraints = {
		area bindable: false, display: false
		areaMeters display: false, default: 10 // Default gardens to 10 m**2
 		postCode()
		yearsGrowing()

		isOrganic()
		submittingAllData()
		consentGiven validator: { value, object ->
			value == true
		}
		electronicSignature()
	}
}
