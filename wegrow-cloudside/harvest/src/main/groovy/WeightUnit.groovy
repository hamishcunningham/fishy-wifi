package harvest

public enum WeightUnit {
  KILOGRAMMES('kg'),
  OUNCES('oz')

  String name

  WeightUnit(String name) {
    this.name = name
  }

  public static WeightUnit getDefault() {
    KILOGRAMMES
  }

  public Double normalise(Double weight) {
    switch (this) {
      case KILOGRAMMES:
        return weight * 1000;
      case OUNCES:
        return weight * 28.3495;
    }
  }

  public Double denormalise(Double weight) {
    switch (this) {
      case KILOGRAMMES:
        return weight / 1000;
      case OUNCES:
        return weight / 28.3495;
    }
  }
}
