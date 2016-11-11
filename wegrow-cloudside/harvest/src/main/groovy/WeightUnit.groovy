package harvest

public enum WeightUnit {
  GRAMMES('Grammes'),
  OUNCES('Ounces')

  String name
 
  WeightUnit(String name) {
    this.name = name
  }
}
