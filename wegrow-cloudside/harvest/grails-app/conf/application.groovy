
// Added by the Spring Security Core plugin:
grails.plugin.springsecurity.userLookup.userDomainClassName = 'harvest.User'
grails.plugin.springsecurity.userLookup.authorityJoinClassName = 'harvest.UserRole'
grails.plugin.springsecurity.authority.className = 'harvest.Role'

//grails.plugin.springsecurity.controllerAnnotations.staticRules = [
grails.plugin.springsecurity.securityConfigType = "InterceptUrlMap"
grails.plugin.springsecurity.interceptUrlMap = [
  [pattern: '/',               access: ['permitAll']],
  [pattern: '/greenmail/**',        access: ['permitAll']],

  [pattern: '/error',          access: ['permitAll']],
  [pattern: '/index',          access: ['permitAll']],
  [pattern: '/index_default.gsp',      access: ['permitAll']],
  [pattern: '/shutdown',       access: ['permitAll']],
  [pattern: '/assets/**',      access: ['permitAll']],
  [pattern: '/**/js/**',       access: ['permitAll']],
  [pattern: '/**/css/**',      access: ['permitAll']],
  [pattern: '/**/images/**',   access: ['permitAll']],
  [pattern: '/**/favicon.ico', access: ['permitAll']],
  [pattern: '/user/**',        access: 'ROLE_ADMIN'],

  [pattern: '/admin/**',       access: ['ROLE_ADMIN', 'isFullyAuthenticated()']],
  [pattern: '/login/**',       access: ['permitAll']],
  [pattern: '/logout/**',      access: ['permitAll']],
  [pattern: '/register/**',    access: ['permitAll']],
  [pattern: '/login/impersonate', access: ['ROLE_ADMIN']],
  [pattern: '/logout/impersonate', access: ['permitAll']],
  [pattern: '/api/login',          access: ['permitAll']],
  [pattern: '/api/logout',        access: ['isFullyAuthenticated()']],
  [pattern: '/api/**',    access: ['isFullyAuthenticated()']],
  [pattern: '/**',             access: ['isFullyAuthenticated()']]
]

grails.plugin.springsecurity.filterChain.chainMap = [
  [pattern: '/assets/**',      filters: 'none'],
  [pattern: '/**/js/**',       filters: 'none'],
  [pattern: '/**/css/**',      filters: 'none'],
  [pattern: '/**/images/**',   filters: 'none'],
  [pattern: '/**/favicon.ico', filters: 'none'],
  [pattern: '/api/**', filters:'JOINED_FILTERS,-anonymousAuthenticationFilter,-exceptionTranslationFilter,-authenticationProcessingFilter,-securityContextPersistenceFilter'],
  [pattern: '/**', filters:'JOINED_FILTERS,-restTokenValidationFilter,-restExceptionTranslationFilter']
]
grails.plugin.springsecurity.roleHierarchy = '''
   ROLE_ADMIN > ROLE_USER
'''

// TODO delete this when logout changed to POST in gsps; see
// https://grails-plugins.github.io/grails-spring-security-core/v3/index.html
grails.plugin.springsecurity.logout.postOnly = false
grails.plugin.springsecurity.ui.register.defaultRoleNames = ['ROLE_USER']
grails.plugin.springsecurity.ui.register.postRegisterUrl = '/growingSpace/create'


grails.plugin.springsecurity.rest.logout.endpointUrl = '/api/logout'
grails.plugin.springsecurity.rest.token.validation.useBearerToken = false
grails.plugin.springsecurity.rest.token.validation.headerName = 'X-Auth-Token'

//does this point at something actually running?
grails.plugin.springsecurity.rest.token.storage.memcached.hosts = 'localhost:11211'
grails.plugin.springsecurity.rest.token.storage.memcached.username = ''
grails.plugin.springsecurity.rest.token.storage.memcached.password = ''
grails.plugin.springsecurity.rest.token.storage.memcached.expiration = 86400

grails {
    mail {
        host = "smtp.gmail.com"
        port = 465
        username = harvest.Passwords.EMAIL_ADDR
        password = harvest.Passwords.EMAIL_PWD
        props = ["mail.smtp.auth":"true",
                 "mail.smtp.socketFactory.port":"465",
                 "mail.smtp.socketFactory.class":"javax.net.ssl.SSLSocketFactory",
                 "mail.smtp.socketFactory.fallback":"false"]
    }
}


environments {
    production {
        grails {
            serverURL = "https://submit.myharvest.org.uk"
        }
        grails.plugin.databasemigration.updateOnStart = true
        grails.plugin.databasemigration.updateOnStartFileName = 'changelog.groovy'
    }

    development {
        grails.serverURL = "http://localhost:8080/"
    }
    test {
        grails.serverURL = "http://localhost:8080/"
    }
}

