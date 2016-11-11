package harvest

class Harvest {
  Crop crop
  int weight
  WeightUnit unit

  static constraints = {
    crop()
    weight()
    unit()
  }
}
