package harvest

class Harvest {
  Double weight
  WeightUnit unit
  transient springSecurityService

  Double weightGrammes

  static belongsTo = [area:Area]
  static transients = ['weight', 'unit', 'springSecurityService']

  static constraints = {
    area()
    weight(greaterThan: new Double(0.0), bindable: true, display: true)
    weightGrammes greaterThan: new Double(0.0), display: false
    unit display: false, bindable: true

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


}
