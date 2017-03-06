package harvest

import grails.util.Environment

class BootStrap {

  def initDevelopment = { servletContext ->
    // create user(s) if none exist
    if (!User.list()) {
      def adminRole = new Role(authority: 'ROLE_ADMIN').save()
      def userRole = new Role(authority: 'ROLE_USER').save()

      def testUser = new User(username: 'me', password: 'x', email: "me@example.com").save()
      UserRole.create testUser, userRole

      def adminUser = new User(username: 'admin', password: 'x', email:"admin@example.com").save()
//      def garden = new Garden(areaMeters:100, yearsGrowing:25, isOrganic: false,
//              submittingAllData: false, consentGiven: true, postCode: "S6", electronicSignature:"dom");
//
//      testUser.growingSpace = garden
//      testUser.save()

      UserRole.create adminUser, adminRole

      UserRole.withSession {
        it.flush()
        it.clear()
      }

      // Create the empty settings entry (we only ever want to have one).
      new Settings().save()

    }

    // create some crops if none exist
    if (!Crop.list()) {
      initCrops(servletContext)
    }

  }
  def init = { servletContext ->
    if (Environment.current == Environment.DEVELOPMENT) {
      initDevelopment(servletContext)
    } else if (Environment.current == Environment.PRODUCTION) {
        initProduction(servletContext)
    }
  }
  def destroy = {
  }
  def initProduction = { servletContext ->
      if (!User.list()) {
        def adminRole = new Role(authority: 'ROLE_ADMIN').save()
        def userRole = new Role(authority: 'ROLE_USER').save()
        def adminUser = new User(username: 'admin', password: harvest.Passwords.ADMIN_PWD,
                email: harvest.Passwords.EMAIL_ADDR).save()
        UserRole.create adminUser, adminRole
        UserRole.withSession {
          it.flush()
          it.clear()
        }

        new Settings().save()
      }
      if (!Crop.list()) {
        initCrops(servletContext)
      }
    }

    def initCrops = { servletContext ->
      def crops = [new Crop(type:"Potatoes"),
                   new Crop(type:"Beetroot"),
                   new Crop(type:"Carrots"),
                   new Crop(type:"Parsnips"),
                   new Crop(type:"Turnips / Swedes"),
                   new Crop(type:"Cabbage"),
                   new Crop(type:"Brussel sprouts"),
                   new Crop(type:"Broccoli"),
                   new Crop(type:"Onions"),
                   new Crop(type:"Leeks"),
                   new Crop(type:"French / climbing beans"),
                   new Crop(type:"Runner beans"),
                   new Crop(type:"Broad beans"),
                   new Crop(type:"Peas"),
                   new Crop(type:"Courgettes"),
                   new Crop(type:"Squash / Pumpkins"),
                   new Crop(type:"Sweetcorn"),
                   new Crop(type:"Lettuce / salad leaves"),
                   new Crop(type:"Tomatoes", isGreenhouseable: true),
                   new Crop(type:"Currants", isGreenhouseable: true),
                   new Crop(type:"Strawberries", isGreenhouseable: true),
                   new Crop(type:"Raspberries", isGreenhouseable: true),
                   new Crop(type:"Gooseberries", isGreenhouseable: true),
                   new Crop(type:"Apples", isTree: true),
                   new Crop(type:"Plums", isTree: true)]
      for(crop in crops) {
        if(!crop.save() || crop.hasErrors()) {
          log.error("Bootstrapped crop has errors: ${crop.properties}")
        }
      }

    }
}
