package harvest

public enum LengthUnit {
  METERS('m'),
  FEET('ft')

  String name

  LengthUnit(String name) {
    this.name = name
  }

  public static LengthUnit getDefault() {
    METERS
  }
  /**
   * Convert units to meters for normalised storage in the database
   * @param length
   * @return
     */
  public Double normalise(Double length) {
    switch (this) {
      case METERS:
        return length;
      case FEET:
        return length * 0.3048;
    }
  }

  /**
   * Converts from meters back into this unit.
   * @param length
   * @return
     */
  public Double denormalise(Double length) {
    switch (this) {
      case METERS:
        return length;
      case FEET:
        return length / 0.3048;
    }
  }
}
