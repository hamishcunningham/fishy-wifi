package harvest

class Crop {
  String type
  Boolean isTree = false
  Boolean isGreenhouseable = false

  static hasMany = ["areas":Area]
  static constraints = {
    type()
  }
  String toString() { return type }
}
