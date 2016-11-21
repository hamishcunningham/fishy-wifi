package harvest

import static grails.plugin.springsecurity.SpringSecurityUtils.ifAllGranted

class FirstAreaInterceptor {
    def springSecurityService
    int order = LOWEST_PRECEDENCE - 20

    FirstAreaInterceptor() {
        match(controller: "harvest")
        match(controller: "growingSpace", action:"show")
        match(controller: "garden", action:"show")
        match(controller: "allotment", action:"show")
        match(controller: "otherGrowingSpace", action:"show")

    }

    boolean before() {
        if (!ifAllGranted("ROLE_ADMIN") &&
                springSecurityService != null &&
                springSecurityService.currentUser != null) {
            def growingSpace = springSecurityService.currentUser.growingSpace

            if (growingSpace != null && (growingSpace.areas == null || growingSpace.areas.size() == 0)) {
                flash.message = "Please tell us about the area you will be using to grow a crop."

                redirect(controller:"area", action: "create")
            } else {
                true
            }
        } else {
            true
        }
    }

    boolean after() { true }

    void afterView() {
        // no-op
    }
}
