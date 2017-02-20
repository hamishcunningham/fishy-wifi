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

  List<Harvest> getHarvests() {
    areas.collectMany { area -> area.harvests }
  }

  def getYield() {
    def yields = harvests.collectMany { harvests.yield }

    if (yields.isEmpty()) {
      return 0 // Yields may be entry which would be a divide by zero. Just assume 0/0 is 0 for now.
    } else {
      return yields.sum() / yields.size()
    }
  }
}
