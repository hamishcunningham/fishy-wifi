package harvest

import grails.plugin.springsecurity.annotation.Secured

/**
 * Created by dominic on 23/11/2016.
 */
@Secured("ROLE_USER")
class UserController {
    def springSecurityService

    def editPreferences() {
        User user = springSecurityService.currentUser
        [       "preferences": ["preferredAreaUnit", "preferredWeightUnit"], // Only show these fields as preferences
                "user": user
        ]
    }
    def updatePreferences(User user) {
        User currentUser = springSecurityService.currentUser
        currentUser.preferredAreaUnit = user.preferredAreaUnit
        currentUser.preferredWeightUnit = user.preferredWeightUnit
        currentUser.save()
        flash.message = 'User preferences saved'
        render view: 'editPreferences', model:[       "preferences": ["preferredAreaUnit", "preferredWeightUnit"], // Only show these fields as preferences
                                                      "user": currentUser]

    }

    def changePassword() {
        [username: springSecurityService.principal.username]
    }

    def updatePassword(String password, String password_new, String password_new_2) {
        String username = springSecurityService.principal.username
        if (!username) {
            flash.message = 'Sorry, an error has occurred'
            redirect controller: 'main', action: 'index'
            return
        }

        if (!password || !password_new || !password_new_2) {
            flash.message = 'Please enter your current password and a valid new password'
            render view: 'changePassword', model: [username: springSecurityService.principal.username]
            return
        }
        if (password_new != password_new_2) {
            flash.message = 'Passwords do not match'
            render view: 'changePassword', model: [username: springSecurityService.principal.username]
            return
        }


        User user = User.findByUsername(username)
        if (!springSecurityService.passwordEncoder.isPasswordValid(user.password, password, null /*salt*/)) {
            flash.message = 'Current password is incorrect'
            render view: 'changePassword', model: [username: springSecurityService.principal.username]
            return
        }

        user.password = password_new
        user.passwordExpired = false
        user.save() // if you have password constraints check them here

        flash.message = "Password changed successfully"
        flash.messageType = "success"
        redirect "/"
    }
}
