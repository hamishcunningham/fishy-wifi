package harvest

import grails.plugin.springsecurity.SpringSecurityUtils
import org.hibernate.LazyInitializationException

class Area {
  Double areaMeters
  transient springSecurityService
  private Double area;
  private harvest.AreaUnit unit;
  private canopyRadius
  Double canopyRadiusMeters

  String variety
  String name
  Boolean inGreenhouse
  private harvest.LengthUnit radiusUnit;


  Boolean finished

  def afterInsert() {
    if (space == null) {
      targetSpace = springSecurityService.currentUser.growingSpace
      targetSpace.addToAreas(this)
      targetSpace.save()
    }
  }

  static transients = ['area',
                       'unit',
                      'canopyRadius',
                       'radiusUnit',
                       'springSecurityService']

  static belongsTo = [space:GrowingSpace, crop:Crop]
  static hasMany = [harvests: Harvest]

  static constraints = {
    name()
    crop()
    variety nullable: true
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
    canopyRadiusMeters bindable: true, display: true, nullable: true, blank: true, validator: { Double val, Area obj ->
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

    canopyRadius bindable: true, display: true, nullable: true, blank: true
    radiusUnit bindable: true, display: true, nullable: true, blank: true


    finished nullable: true
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

  harvest.AreaUnit getUnit() {
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

  def setCanopyRadius(radius) {
    this.canopyRadius = radius
    if (radius != null && this.radiusUnit != null) {
      this.canopyRadiusMeters = radiusUnit.normalise(radius)
    }
  }

  Double getCanopyRadius() {
    User currentUser = springSecurityService.currentUser
    if (this.canopyRadius != null) {
      return this.canopyRadius
    } else if (canopyRadiusMeters != null) {
      return getRadiusUnit().denormalise(canopyRadiusMeters)
    } else {
      return canopyRadiusMeters
    }
  }

  harvest.LengthUnit getRadiusUnit() {
    if (this.radiusUnit != null) {
      return this.radiusUnit
    } else if (springSecurityService.currentUser != null) {
      return springSecurityService.currentUser?.preferredLengthUnit
    } else {
      return LengthUnit.default
    }
  }

  def setRadiusUnit(radiusUnit) {
    this.radiusUnit = radiusUnit
    if (canopyRadius != null && radiusUnit != null) {
      canopyRadiusMeters = radiusUnit.normalise(canopyRadius)
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
        space == growingSpace && (finished != true)
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
      if (name) {
        return "${name}";
      }
      else {
        return "${crop?.type} (${variety})";
      }
    } catch (LazyInitializationException e) {
      return "${getArea()}${getUnit().name}"
    }
  }

  def getYield() {
    def weights = harvests.weightGrammes

    def m2 = (!crop.isTree ? // Use the radius to get the area for trees.
            areaMeters : (canopyRadiusMeters ** 2) * Math.PI)

    if (m2 > 0 && weights) {
      return weights.sum() / m2
    } else {
      return 0
    }

  }

  def getAmountGrammes() {
    def weights = harvests.weightGrammes
    return weights.sum() ?: 0
  }

  def getAmount() {
    return amountUnit.denormalise(amountGrammes)
  }

  harvest.WeightUnit getAmountUnit() {
      return springSecurityService.currentUser?.preferredWeightUnit
  }

}
