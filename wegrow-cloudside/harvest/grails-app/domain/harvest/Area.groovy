package harvest

import grails.plugin.springsecurity.SpringSecurityUtils
import org.hibernate.LazyInitializationException

class Area {
  Double areaMeters
  transient springSecurityService
  private Double area;
  private harvest.AreaUnit unit;
  Double canopyRadius
  Boolean inGreenhouse

  Boolean finished = false

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
    areaMeters display: false, nullable: true, validator: { Double val, Area obj ->
      if (obj.crop.isTree) {
        if (val) {
          return "isTree"
        } else {
          return true
        }
      }
      if (!obj.crop.isTree) {
        if (val) {
          return true
        }
        else {
          return "nullable"
        }
      }
      return false
    }
    area bindable: true, display: true, nullable: true, blank: true

    // Only trees have a canopy radius
    canopyRadius bindable: true, display: true, nullable: true, blank: true, validator: { Double val, Area obj ->
      if (obj.crop.isTree) {
        if (val) {
          return true
        } else {
          return "nullable"
        }
      }
      if (!obj.crop.isTree) {
        if (!val) {
          return true
        }
        else {
          return "isNotTree"
        }
      }
      return false
    }

    // Only certain crops are sometimes grown indoors.
    inGreenhouse nullable: true, blank: true, validator: { Boolean val, Area obj ->
      if (obj.crop.isGreenhouseable) {
        if (val != null) {
          return true
        } else {
          return "nullable"
        }
      }
      if (!obj.crop.isGreenhouseable) {
        if (!val) {
          return true
        }
        else {
          return "isNotGreenhouseable"
        }
      }
      return false
    }

    unit display: false, bindable: true, nullable: true
    finished display: false, nullable: true
    harvests display: false


  }

  static mapping = {
    finished column: "finished", defaultValue: false
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

  /**
   * Provides a list of the areas that belong to the current user and are not finished.
   * @param currentUser
   * @return
     */
  static def selectableAreas(currentUser) {
    if (currentUser?.growingSpace != null) {
      def growingSpace = currentUser.growingSpace

      return Area.where {
        space == growingSpace && finished == false
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
      if (crop?.isTree) {
        return "${crop?.type} with canopy ${getCanopyRadius()}";
      }
      else {
        return "${crop?.type} growing in ${getArea()}${getUnit().name}";
      }
    } catch (LazyInitializationException e) {
      return "${getArea()}${getUnit().name}"
    }
  }
}
