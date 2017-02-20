package harvest

import grails.plugin.springsecurity.SpringSecurityUtils
import grails.plugin.springsecurity.annotation.Secured
import grails.transaction.Transactional

import static org.springframework.http.HttpStatus.NO_CONTENT

/**
 * Created by dominic on 23/11/2016.
 */
@Secured("hasRole('ROLE_USER')")
class UserController {
    def springSecurityService
    def exportService
    @Secured("hasRole('ROLE_ADMIN')")
    def index(Integer offset, Integer max) {
        if (params?.f) {
//Username	Email	Organic	Signature	Years growing	Months growing	All data?	Location
            def fields = ["username", "email", "organic", "signature", "yrs_growing",
                          "mnths_growing", "all_data", "location"]
            def data = User.list().collect { u ->
                [
                        username: u.username,
                        email: u.email,
                        organic: u.growingSpace?.isOrganic,
                        signature: u.growingSpace?.electronicSignature,
                        yrs_growing: u.growingSpace?.yearsGrowing,
                        mnths_growing: u.growingSpace?.monthsGrowing,
                        all_data: u.growingSpace?.submittingAllData,
                        location: u.growingSpace?.locationString,
                        id: u.id
                ]
            }
            response.contentType = grailsApplication.config.grails.mime.types[params.f]
            response.setHeader("Content-disposition", "attachment; filename=users.${params.extension}")
            exportService.export(params.f, response.outputStream, data, fields, [:],[:],[:])
        } else {
            max = max ?: 10
            offset = offset != null ? offset : 0

            respond userList: User.list(max: max, offset: offset), userCount: User.count()
        }
    }


    @Transactional
    def delete(User user) {

        if (user == null) {
            transactionStatus.setRollbackOnly()
            notFound()
            return
        }

        if (!SpringSecurityUtils.ifAllGranted("ROLE_ADMIN")) {
            transactionStatus.setRollbackOnly()
            notFound()
            return
        }

        if (user == springSecurityService.currentUser) {
            transactionStatus.setRollbackOnly()
            flash.message = "User are not allowed to delete themselves."
            redirect action: "index"
            return
        }
        def userRoles = UserRole.findAllByUser(user)

        userRoles*.delete()
        user.delete()


        request.withFormat {
            form multipartForm {
                flash.message = message(code: 'default.deleted.message', args: [message(code: 'user.label', default: 'User'), user.id])
                redirect action:"index", method:"GET"
            }
            '*'{ render status: NO_CONTENT }
        }
    }

    def editPreferences() {
        User user = springSecurityService.currentUser
        [       "preferences": ["preferredAreaUnit", "preferredWeightUnit", "preferredLengthUnit"], // Only show these fields as preferences
                "user": user
        ]
    }
    def updatePreferences(User user) {
        User currentUser = springSecurityService.currentUser
        currentUser.preferredAreaUnit = user.preferredAreaUnit
        currentUser.preferredWeightUnit = user.preferredWeightUnit
        currentUser.preferredLengthUnit = user.preferredLengthUnit
        currentUser.save()
        flash.message = 'User preferences saved'
        render view: 'editPreferences', model:["preferences": ["preferredAreaUnit", "preferredWeightUnit", "preferredLengthUnit"], // Only show these fields as preferences
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
