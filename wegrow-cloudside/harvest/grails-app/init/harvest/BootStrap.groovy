package harvest

class BootStrap {

  def init = { servletContext ->

    // create user(s) if none exist
    if (!User.list()) {
      def adminRole = new Role(authority: 'ROLE_ADMIN').save()
      def userRole = new Role(authority: 'ROLE_USER').save()

      def testUser = new User(username: 'me', password: 'x').save()
      UserRole.create testUser, userRole

      def adminUser = new User(username: 'admin', password: 'x').save()
      def garden = new Garden(areaMeters:100, yearsGrowing:25, isOrganic: false,
              submittingAllData: false, consentGiven: true, electronicSignature:"dom");

      testUser.growingSpace = garden;
      testUser.save()

      UserRole.create adminUser, adminRole

      UserRole.withSession {
        it.flush()
        it.clear()
      }

      // Create the empty settings entry (we only ever want to have one).
      new Settings().save()

//      assert User.count() == 2
//      assert Role.count() == 2
//      assert UserRole.count() == 2
    }

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
