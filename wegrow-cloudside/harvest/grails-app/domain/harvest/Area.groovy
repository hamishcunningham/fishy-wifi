package harvest

class Area {
  Double areaMeters
  Double area
  AreaUnit unit
  transient springSecurityService

  def afterInsert() {
    if (space == null) {
      targetSpace = springSecurityService.currentUser.growingSpace
      targetSpace.addToAreas(this)
      targetSpace.save()
    }
  }

  static transients = ['area', 'unit', 'springSecurityService']
  static belongsTo = [space:GrowingSpace, crop:Crop]

  static constraints = {
    space display: false
    areaMeters display: false
    area bindable: true, display: true
    unit display: false

  }

  def beforeValidate() {
    if (area != null && unit != null) {
      areaMeters = unit.normalise(area)
    }

    if (space == null) {
      def targetSpace = springSecurityService.currentUser.growingSpace
      targetSpace.addToAreas(this)
    }
  }


  @Override
  public String toString() {
    return "${crop?.type} growing in ${areaMeters}\u33A1";
  }
}
