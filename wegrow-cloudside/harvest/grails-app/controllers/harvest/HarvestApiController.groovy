package harvest;

import grails.rest.RestfulController;
import grails.transaction.Transactional

import grails.plugin.springsecurity.SpringSecurityService
import grails.plugin.springsecurity.annotation.Secured

@Transactional(readOnly = true)
@Secured("hasRole('ROLE_USER')")
class HarvestApiController extends RestfulController {

	SpringSecurityService springSecurityService;

	HarvestApiController() {
		super(Harvest)
	}
	
	def index() {
		
	    User currentUser = springSecurityService.currentUser
	    def criteria = Harvest.createCriteria()
	    
	    if (params?.area) {
		    	    
		    def results = criteria.list {
		    	eq("area.id", Long.valueOf(params.area))                  
			}
		
			respond results
		}
		else {
		    def results = criteria.list {
		    	'in'("area", currentUser?.growingSpace.areas)                  
			}
		
			respond results
		}
	}
}
