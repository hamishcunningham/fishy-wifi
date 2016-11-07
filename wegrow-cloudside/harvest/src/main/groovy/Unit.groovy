package harvest

public enum Unit {
  SQ_FEET('Square feet'),
  SQ_METERS('Square meters')

  String name
 
  Unit(String name) {
    this.name = name
  }
}
