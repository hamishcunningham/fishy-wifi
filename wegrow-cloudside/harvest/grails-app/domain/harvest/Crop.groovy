package harvest

class Crop {
  String type

  static hasMany = ["areas":Area]
  static constraints = {
    type()
  }
  String toString() { return type }
}
