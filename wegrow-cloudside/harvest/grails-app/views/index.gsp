<!DOCTYPE html>
<html>
    <head>
        <meta name="layout" content="main" />
        <g:set var="entityName" value="${message(code: 'site.label', default: 'GrowingSpace')}" />
        <title><g:message code="default.create.label" args="[entityName]" /></title>
    </head>
<body>
<g:ifHydro>
 	<sec:ifLoggedIn>
 		<p class="text-center lead">
 			Welcome to myHarvest for Hydroponics.
 		</p>
 	</sec:ifLoggedIn>    
    <sec:ifNotLoggedIn>
        <p class="text-center lead">
            Welcome to myHarvest for Hydroponics.
        </p>

        <p class="text-center">
            <g:link controller="register" action="register" class="btn btn-default btn-lg"><g:message code="register" default="Create an Account"/></g:link>
            or
            <g:link controller="login" action="auth" class="btn btn-default btn-lg"><g:message code="login" default="Login"/></g:link>

        </p>
    </sec:ifNotLoggedIn>
</g:ifHydro>
<g:ifConventional>
    <g:img file="big_logo_conventional.png" class="big_logo center-block"/>

        <sec:ifLoggedIn>
            <p class="text-center lead">
                Welcome to myHarvest, <sec:username/>.
            </p>

            <p class="text-center">
                To tell us about what crops you are growing and what you have harvested, click the button below
            </p>
            <p class="text-center">
                <g:link controller="harvest" action="index" class="btn btn-default btn-lg"><g:message code="harvest.title" /></g:link>
            </p>

        </sec:ifLoggedIn>
        <sec:ifNotLoggedIn>
            <p class="text-center lead">
                Welcome to myHarvest.
            </p>

            <p class="text-center">
                <g:link controller="register" action="register" class="btn btn-default btn-lg"><g:message code="register" default="Create an Account"/></g:link>
                or
                <g:link controller="login" action="auth" class="btn btn-default btn-lg"><g:message code="login" default="Login"/></g:link>

            </p>
        </sec:ifNotLoggedIn>
        </g:ifConventional>
    </body>
</html>