package harvest;

import grails.rest.RestfulController;
import grails.transaction.Transactional

import grails.plugin.springsecurity.SpringSecurityService
import grails.plugin.springsecurity.annotation.Secured

@Transactional(readOnly = true)
@Secured("hasRole('ROLE_USER')")
class AreaApiController extends RestfulController {

	HarvestService harvestService

	SpringSecurityService springSecurityService;

	AreaApiController() {
		super(Area)
	}
	
	def index() {
		
		//def visible = Area.visibleAreas(springSecurityService.currentUser, false)
	    
	    //respond visible.list()
	    
	    
	    User currentUser = springSecurityService.currentUser
	    
	    def criteria = Area.createCriteria()
	    
	    def results = criteria.list {
	    	eq("space", currentUser?.growingSpace)                  
		}
		
		respond results

	}
}
