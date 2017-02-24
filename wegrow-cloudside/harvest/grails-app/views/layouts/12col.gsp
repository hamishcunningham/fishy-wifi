<!doctype html>
<html lang="en" class="no-js">
<head>
    <meta http-equiv="Content-Type" content="text/html; charset=UTF-8"/>
    <meta http-equiv="X-UA-Compatible" content="IE=edge"/>
    <title>
        <g:layoutTitle default="Grails"/>
    </title>
    <meta name="viewport" content="width=device-width, initial-scale=1, maximum-scale=1, user-scalable=no"/>
    <r:require module="export"/>
    <asset:stylesheet src="application.css"/>

    <g:ifHydro>
        <asset:stylesheet src="application_hydro.css"/>
    </g:ifHydro>

    <g:ifConventional>
        <asset:stylesheet src="application_conventional.less"/>
    </g:ifConventional>


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
                <g:link class="navbar-brand" uri="/">
                    <img src="${asset.assetPath(src: "LOGO/WEB/MyHarvest-WEB.png")}" alt="website logo"  />
                </g:link>

            </div>

            <div class="collapse navbar-collapse" id="bs-example-navbar-collapse-1">

                <ul class="nav navbar-nav navbar-right">
                    %{--<li id="spinner" style="display:none;">--}%
                        %{--<g:message code="spinner.alt" default="Loading&hellip;"/>--}%
                    %{--</li>--}%

                    <sec:ifNotLoggedIn>
                         <li>
                            <g:link resource="login" action="auth" ><g:message code="login" default="Login" /></g:link>
                         </li>
                          <li>
                             <g:link resource="register" action="register" ><g:message code="register" default="Register" /></g:link>
                          </li>
                    </sec:ifNotLoggedIn>
                    <sec:ifLoggedIn>

                        <sec:access expression="hasRole('ROLE_ADMIN')">
                            <li>
                                <g:link resource="crop" action="index">
                                    <g:message code="crops.title" default="Manage Crops" />
                                </g:link>
                            </li>
                        </sec:access>

                        <li>
                            <g:link resource="harvest" action="index" >
                                 <g:message code="harvest.title" default="Past Harvests" />
                            </g:link>
                         </li>

                         <li>
                             <p class="navbar-text">
                                Logged in as <sec:loggedInUserInfo field="username"/>
                             </p>
                         </li>
                         <li>
                            <g:link resource="user" action="editPreferences">
                                         <g:message code="preferences" default="Preferences" />
                            </g:link>
                         </li>



                         <sec:access expression="hasRole('ROLE_ADMIN')">
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
        <div class="container-fluid">
            <div class="col-md-12">
                <g:layoutBody/>
            </div>
        </div>
    </div>
    <div class="footer" role="contentinfo"></div>


    <asset:javascript src="application.js"/>

</body>
</html>
