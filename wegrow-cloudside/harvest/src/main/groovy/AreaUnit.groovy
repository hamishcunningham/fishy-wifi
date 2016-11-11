package harvest

public enum AreaUnit {
  SQ_METERS('Square meters'),
  SQ_FEET('Square feet')

  String name
 
  AreaUnit(String name) {
    this.name = name
  }
}
