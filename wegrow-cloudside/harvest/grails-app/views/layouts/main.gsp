<!doctype html>
<html lang="en" class="no-js">
<head>
    <meta http-equiv="Content-Type" content="text/html; charset=UTF-8"/>
    <meta http-equiv="X-UA-Compatible" content="IE=edge"/>
    <title>
        <g:layoutTitle default="Grails"/>
    </title>
    <meta name="viewport" content="width=device-width, initial-scale=1"/>

    <asset:stylesheet src="application.css"/>

    <g:layoutHead/>
</head>
<body>

    <div class="navbar navbar-default navbar-static-top" role="navigation">
        <div class="container-fluid">
            <div class="navbar-header">
                 <button type="button" class="navbar-toggle collapsed" data-toggle="collapse" data-target="#bs-example-navbar-collapse-1" aria-expanded="false">
                     <span class="sr-only">Toggle navigation</span>
                     <span class="icon-bar"></span>
                     <span class="icon-bar"></span>
                     <span class="icon-bar"></span>
                </button>
                <a class="navbar-brand" href="#"><g:message code="app.name" default="${meta(name:'info.app.name')}" /></a>
            </div>

            <div class="collapse navbar-collapse" id="bs-example-navbar-collapse-1">
                <ul class="nav navbar-nav navbar-right">
                    <sec:ifNotLoggedIn>
                         <li>
                            <g:link resource="login" action="auth" ><g:message code="login" default="Login" /></g:link>
                         </li>
                          <li>
                             <g:link resource="register" action="register" ><g:message code="Register" default="Register" /></g:link>
                          </li>
                    </sec:ifNotLoggedIn>
                    <sec:ifLoggedIn>
                         <li>
                             <p class="navbar-text">
                                Logged in as <sec:loggedInUserInfo field="username"/>
                             </p>
                         </li>
                         <sec:access expression="hasRole('ROLE_USER')">
                             <li>
                                <g:link resource="admin" ><g:message code="admin" default="Admin" /></g:link>
                             </li>
                         </sec:access>
                         <li>
                            <g:link resource="logout" ><g:message code="logout" default="Logout" /></g:link>
                         </li>
                    </sec:ifLoggedIn>
                </ul>
            </div>

        </div>

    </div>

    <div class="row">
        <div class="container">
            <g:layoutBody/>
        </div>
    </div>
    <div class="footer" role="contentinfo"></div>

    <div id="spinner" class="spinner" style="display:none;">
        <g:message code="spinner.alt" default="Loading&hellip;"/>
    </div>

    <asset:javascript src="application.js"/>

</body>
</html>
