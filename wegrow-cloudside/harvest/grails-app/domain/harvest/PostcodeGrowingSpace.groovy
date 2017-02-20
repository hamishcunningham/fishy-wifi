package harvest

class PostcodeGrowingSpace extends GrowingSpace {
	String postCode

	@Override
	String getLocationString() {
		return postCode
	}

}
