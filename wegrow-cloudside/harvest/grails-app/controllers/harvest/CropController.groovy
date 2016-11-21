package harvest

import grails.plugin.springsecurity.annotation.Secured

import static org.springframework.http.HttpStatus.*
import grails.transaction.Transactional

@Transactional(readOnly = true)
@Secured("hasRole('ROLE_ADMIN')")
class CropController {

  static scaffold = Crop
}
