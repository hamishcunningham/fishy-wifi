package harvest

class UrlMappings {

    static mappings = {
    
    	//I don't think this should be nesseary but...
    	//it's the only way I can get it to work
    	"/api/$controller/$action?/$id?"(controller:"harvest") {
    		format = "json"
    	}

    
        "/$controller/$action?/$id?(.$format)?"{
            constraints {
                // apply constraints here
            }
        }

        "/"(view:"/index")
        "500"(view:'/error')
        "404"(view:'/notFound')
    }
}
