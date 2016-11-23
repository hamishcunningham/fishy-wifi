package harvest

public enum WeightUnit {
  GRAMMES('g'),
  OUNCES('oz')

  String name

  WeightUnit(String name) {
    this.name = name
  }

  public static WeightUnit getDefault() {
    GRAMMES
  }

  public Double normalise(Double weight) {
    switch (this) {
      case GRAMMES:
        return weight;
      case OUNCES:
        return weight * 28.3495;
    }
  }

  public Double denormalise(Double weight) {
    switch (this) {
      case GRAMMES:
        return weight;
      case OUNCES:
        return weight / 28.3495;
    }
  }
}
