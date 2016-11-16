package harvest

public enum AreaUnit {
  SQ_METERS('Square meters'),
  SQ_FEET('Square feet')

  String name
 
  AreaUnit(String name) {
    this.name = name
  }

  /**
   * Convert units to meters for normalised storage in the database
   * @param area
   * @return
     */
  public Double normalise(Double area) {
    switch (this) {
      case SQ_METERS:
        return area;
      case SQ_FEET:
        return area * 0.3048;
    }
  }
}
