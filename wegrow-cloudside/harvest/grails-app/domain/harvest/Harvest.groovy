package harvest

class Harvest {
  Crop crop
  Integer weight
  WeightUnit unit

  static constraints = {
    crop()
    weight(notEqual: 0)
    unit()
  }
}
