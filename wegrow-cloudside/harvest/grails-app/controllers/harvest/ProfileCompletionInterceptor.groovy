package harvest

import static grails.plugin.springsecurity.SpringSecurityUtils.ifAllGranted

class ProfileCompletionInterceptor {
    def springSecurityService
    ProfileCompletionInterceptor() {
        matchAll()
        .excludes(controller: "growingSpace")
        .excludes(controller: "allotment")
        .excludes(controller: "otherGrowingSpace")
        .excludes(controller: "garden")
    }

    boolean before() {
        if (springSecurityService != null && springSecurityService.currentUser != null) {
            if (springSecurityService.currentUser.growingSpace == null && !ifAllGranted("ROLE_ADMIN")) {
                redirect(controller:"growingSpace", action: "create")
                false
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
