package harvest

import static grails.plugin.springsecurity.SpringSecurityUtils.ifAllGranted

class FirstAreaInterceptor {
    def springSecurityService
    int order = LOWEST_PRECEDENCE - 20

    FirstAreaInterceptor() {
        matchAll()
            .excludes(controller: "growingSpace", action:"create")
            .excludes(controller: "allotment", action:"create")
            .excludes(controller: "otherGrowingSpace", action:"create")
            .excludes(controller: "garden", action:"create")
            .excludes(controller: "area", action:"create")
            .excludes(controller: "growingSpace", action:"save")
            .excludes(controller: "allotment", action:"save")
            .excludes(controller: "otherGrowingSpace", action:"save")
            .excludes(controller: "garden", action:"save")
            .excludes(controller: "area", action:"save")
    }

    boolean before() {
        if (!ifAllGranted("ROLE_ADMIN") &&
                springSecurityService != null &&
                springSecurityService.currentUser != null) {
            def growingSpace = springSecurityService.currentUser.growingSpace

            if (growingSpace.areas.size() == 0) {
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
