package harvest

import static grails.plugin.springsecurity.SpringSecurityUtils.ifAllGranted

class FirstAreaInterceptor {
    def springSecurityService
    int order = LOWEST_PRECEDENCE - 20

    FirstAreaInterceptor() {
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
                redirect(controller:"harvest", action: "index")
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
