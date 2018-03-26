package harvest

public enum VolumeUnit {
  CUBIC_METRES('m³'),
  CUBIC_FEET('ft³')

  String name

  VolumeUnit(String name) {
    this.name = name
  }

  public static VolumeUnit getDefault() {
    CUBIC_METRES
  }
  /**
   * Convert units to meters for normalised storage in the database
   */
  public Double normalise(Double volume) {
    switch (this) {
      case CUBIC_METRES:
        return volume;
      case CUBIC_FEET:
        return volume * 0.0283168;
    }
  }

  /**
   * Converts from meters back into this unit.
   */
  public Double denormalise(Double volume) {
    switch (this) {
      case CUBIC_METRES:
        return volume;
      case CUBIC_FEET:
        return volume / 0.0283168;
    }
  }
}
