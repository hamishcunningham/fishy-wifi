package harvest

import static org.springframework.http.HttpStatus.*
import grails.transaction.Transactional

@Transactional(readOnly = true)
class HarvestController {

  static scaffold = Harvest
}
