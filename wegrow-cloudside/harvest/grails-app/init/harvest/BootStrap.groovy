package harvest

class BootStrap {

  def init = { servletContext ->
    // create some crops if none exist
    if (!Crop.list()) {
      log.info("Creating default crops")
      def names = [ "potatoes", "carrots", "kale", "lettuce", ]
      for(n in names) {
        Crop c = new Crop()
        c.properties = [ type: n, ]
        if(!c.save() || c.hasErrors()) {
          log.error("Bootstrapped crop has errors: ${c.properties}")
        }
      }
    }
  }

  def destroy = {
  }
}
