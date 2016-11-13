
// Added by the Spring Security Core plugin:
grails.plugin.springsecurity.userLookup.userDomainClassName = 'harvest.User'
grails.plugin.springsecurity.userLookup.authorityJoinClassName = 'harvest.UserRole'
grails.plugin.springsecurity.authority.className = 'harvest.Role'

grails.plugin.springsecurity.controllerAnnotations.staticRules = [
  [pattern: '/',               access: ['permitAll']],
  [pattern: '/error',          access: ['permitAll']],
  [pattern: '/index',          access: ['permitAll']],
  [pattern: '/index.gsp',      access: ['permitAll']],
  [pattern: '/shutdown',       access: ['permitAll']],
  [pattern: '/assets/**',      access: ['permitAll']],
  [pattern: '/**/js/**',       access: ['permitAll']],
  [pattern: '/**/css/**',      access: ['permitAll']],
  [pattern: '/**/images/**',   access: ['permitAll']],
  [pattern: '/**/favicon.ico', access: ['permitAll']],

  [pattern: '/user/**',        access: 'ROLE_ADMIN'],
  [pattern: '/admin/**',       access: ['ROLE_ADMIN', 'isFullyAuthenticated()']],
]

grails.plugin.springsecurity.filterChain.chainMap = [
  [pattern: '/assets/**',      filters: 'none'],
  [pattern: '/**/js/**',       filters: 'none'],
  [pattern: '/**/css/**',      filters: 'none'],
  [pattern: '/**/images/**',   filters: 'none'],
  [pattern: '/**/favicon.ico', filters: 'none'],
  [pattern: '/**',             filters: 'JOINED_FILTERS']
]

// TODO delete this when logout changed to POST in gsps; see
// https://grails-plugins.github.io/grails-spring-security-core/v3/index.html
grails.plugin.springsecurity.logout.postOnly = false
