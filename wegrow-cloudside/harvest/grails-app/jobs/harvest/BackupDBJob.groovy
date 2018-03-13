package harvest

import javax.sql.DataSource
import java.sql.Statement
import org.springframework.beans.factory.annotation.Value
import grails.config.Config

class BackupDBJob {

	@Value('${harvest.backup.location}')
	String location
	
    static triggers = {
    
    	Config config = grailsApplication.config
    	def trigger = config.getProperty("harvest.backup.trigger")
   
		cron name: 'BackupDB', cronExpression: trigger
    }

	DataSource dataSource

    def execute() {        
    	println "backup to '${location}' started"
        def sql = "SCRIPT DROP TO '${location}'"
        Statement statement = dataSource.connection.createStatement()
        boolean result = statement.execute(sql);
        println "back completed successfully: ${result}"
    }
}
