package harvest

import grails.plugin.springsecurity.SpringSecurityUtils
import org.hibernate.LazyInitializationException

class Area {
  Double areaMeters
  transient springSecurityService
  private Double area;
  private AreaUnit unit;

  def afterInsert() {
    if (space == null) {
      targetSpace = springSecurityService.currentUser.growingSpace
      targetSpace.addToAreas(this)
      targetSpace.save()
    }
  }

  static transients = ['area',
                       'unit',
                       'springSecurityService']

  static belongsTo = [space:GrowingSpace, crop:Crop]
  static hasMany = [harvests: Harvest]

  static constraints = {
    crop()
    space display: false
    areaMeters display: false
    area bindable: true, display: true
    unit display: false, bindable: true
    harvests display: false

  }

  Double getArea() {
    User currentUser = springSecurityService.currentUser
    if (this.area != null) {
      return this.area
    } else if (areaMeters != null) {
      return getUnit().denormalise(areaMeters)
    } else {
      return areaMeters
    }
  }

  AreaUnit getUnit() {
    if (this.unit != null) {
      return this.unit
    } else if (springSecurityService.currentUser != null) {
      return springSecurityService.currentUser?.preferredAreaUnit
    } else {
      return AreaUnit.default
    }
  }

  def setArea(area) {
    this.area = area
    if (area != null && unit != null) {
      areaMeters = unit.normalise(area)
    }
  }

  def setUnit(unit) {
    this.unit = unit
    if (area != null && unit != null) {
      areaMeters = unit.normalise(area)
    }
  }

  def beforeValidate() {
    if (space == null) {
      def targetSpace = springSecurityService.currentUser.growingSpace
      targetSpace.addToAreas(this)
    }
  }

  static def visibleAreas(currentUser) {
    if (SpringSecurityUtils.ifAllGranted("ROLE_ADMIN")) {
      return Area
    } else if (currentUser?.growingSpace != null) {
      def growingSpace = currentUser.growingSpace

      return Area.where {
        space == growingSpace
      }
    }
  }

  def canEdit() {
    if (SpringSecurityUtils.ifAllGranted("ROLE_ADMIN")) {
      return true
    }
    else if (space.user == springSecurityService.currentUser) {
      return true
    }

    return false
  }
  @Override
  public String toString() {
    try {
      return "${crop?.type} growing in ${getArea()}${getUnit().name}";
    } catch (LazyInitializationException e) {
      return "${getArea()}${getUnit().name}"
    }
  }
}
