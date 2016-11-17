package harvest

class Harvest {
  Double weight
  WeightUnit unit

  Double weightGrammes

  static belongsTo = [area:Area]
  static transients = ['weight', 'unit']

  static constraints = {
    area()
    weight(greaterThan: new Double(0.0))
    weightGrammes greaterThan: new Double(0.0), display: false
    unit display: false
  }

  def beforeValidate() {
    if (weight != null && unit != null) {
      weightGrammes = unit.normalise(weight)
    }
  }

}
