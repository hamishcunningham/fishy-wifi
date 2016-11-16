package harvest

class Harvest {
  Crop crop
  Double weight
  WeightUnit unit

  Double weightGrammes

  static belongsTo = [area:Area]
  static transients = ['weight', 'unit']

  static constraints = {
    crop()
    area()
    weight(greaterThan: new Double(0.0), display: false)
    weightGrammes(greaterThan: new Double(0.0))
    unit display: false

  }

  def beforeValidate() {
    if (weight != null) {
      weightGrammes = unit.normalise(weight)
    }
  }

}
