package harvest

public enum AreaUnit {
  SQ_METRES('m²'),
  SQ_FEET('ft²')

  String name

  AreaUnit(String name) {
    this.name = name
  }

  public static AreaUnit getDefault() {
    SQ_METRES
  }
  /**
   * Convert units to meters for normalised storage in the database
   * @param area
   * @return
     */
  public Double normalise(Double area) {
    switch (this) {
      case SQ_METRES:
        return area;
      case SQ_FEET:
        return area * 0.3048;
    }
  }

  /**
   * Converts from meters back into this unit.
   * @param area
   * @return
     */
  public Double denormalise(Double area) {
    switch (this) {
      case SQ_METRES:
        return area;
      case SQ_FEET:
        return area / 0.3048;
    }
  }
}
