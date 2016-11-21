package harvest

import static grails.plugin.springsecurity.SpringSecurityUtils.ifAllGranted

class ProfileCompletionInterceptor {
    def springSecurityService
    ProfileCompletionInterceptor() {
        match(controller: "area")
        match(controller: "harvest")
        match(uri: "/")

    }

    int order = LOWEST_PRECEDENCE - 10

    boolean before() {
        if (!ifAllGranted("ROLE_ADMIN") &&
                springSecurityService != null &&
                springSecurityService.currentUser != null) {
            def growingSpace = springSecurityService.currentUser.growingSpace
            if (growingSpace == null) {
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
