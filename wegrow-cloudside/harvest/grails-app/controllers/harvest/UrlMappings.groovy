package harvest

class UrlMappings {

    static mappings = {
    
    	"/$controller/$action?/$id?(.$format)?"{
            constraints {
                // apply constraints here
            }
        }
    
    	"/api/harvest"(resources:'harvestApi') {
    	    format = "json"
    	}
    	
    	"/api/area"(resources:"areaApi") {
    	    format = "json"
    	}
    	
    	"/api/area/$area/harvests"(resources:"harvestApi", action:"index") {
    		format = "json"                                                        
		}

        "/"(view:"/index")
        "500"(view:'/error')
        "404"(view:'/notFound')
    }
}
