package harvest

import grails.plugin.springsecurity.SpringSecurityUtils

import grails.rest.*

@Resource()
class Harvest {
  Double weight
  WeightUnit unit
  transient springSecurityService
  Date dateCreated
  Date dateHarvested
  Double weightGrammes

  static belongsTo = [area:Area]
  static transients = ['weight', 'unit', 'springSecurityService']

  static constraints = {
    area blank: false, validator: { val, obj -> if (val.space.user != obj.springSecurityService.currentUser) {
        return "wrongUser"
      }
    }

    weight(greaterThan: new Double(0.0), bindable: true, display: true)
    weightGrammes greaterThan: new Double(0.0), display: false
    unit display: false, bindable: true
    dateHarvested display:true, bindable: true

  }

  Double getWeight() {
    User currentUser = springSecurityService.currentUser
    if (this.weight != null) {
      return this.weight
    } else if (weightGrammes != null) {
      return getUnit().denormalise(weightGrammes)
    } else {
      return null
    }
  }

  WeightUnit getUnit() {
    if (this.unit != null) {
      return this.unit
    } else {
      return springSecurityService.currentUser?.preferredWeightUnit
    }
  }

  def setWeight(weight) {
    this.weight = weight
    if (weight != null && unit != null) {
      weightGrammes = unit.normalise(weight)
    }
  }

  def setUnit(unit) {
    this.unit = unit
    if (weight != null && unit != null) {
      weightGrammes = unit.normalise(weight)
    }
  }

  static def visibleHarvests(currentUser) {
    if (SpringSecurityUtils.ifAllGranted("ROLE_ADMIN")) {
      return Harvest
    } else if (currentUser?.growingSpace != null) {
      def growingSpace = currentUser.growingSpace
      return Harvest.where {
        area.space == growingSpace
      }
    }
  }

  def canEdit() {
    if (SpringSecurityUtils.ifAllGranted("ROLE_ADMIN")) {
      return true
    }
    else if (area.space.user == springSecurityService.currentUser) {
      return true
    }

    return false
  }

  def getYield() {
     weightGrammes / (!area.crop.isTree ? // Use the radius to get the area for trees.
             area.areaMeters : (area.canopyRadiusMeters ** 2) * Math.PI)
  }

}
