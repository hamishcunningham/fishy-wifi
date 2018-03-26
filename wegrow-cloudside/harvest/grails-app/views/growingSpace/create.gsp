<!DOCTYPE html>
<html>
    <head>
        <meta name="layout" content="12col" />
        <g:set var="entityName" value="${message(code: 'growingSpace.label', default: 'GrowingSpace')}" />
        <title><g:message code="default.create.label" args="[entityName]" /></title>
    </head>
    <body>
        <div id="create-growingSpace" class="content scaffold-create container" role="main">
            <g:if test="${flash.message}">
                <div class="message" role="status">${flash.message}</div>
            </g:if>


            <div class="growingSpaceSelector">
                <h1 class="display-3">Tell us about you</h1>

                <p class="lead">What type of growing space do you have?</p>
                    <div class="col-lg-4">
                        <g:img class="img-circle" file="allotment_circle.png"></g:img>
                        <h2>Allotment</h2>
                        <p>
                            Your growing space is on the same site as others. Tell us the name of the site and how often you visit.
                        </p>
                        <g:link class="btn btn-default" controller="allotment" action="create" >Get Started</g:link>
                    </div>
                    <div class="col-lg-4">
                        <g:img class="img-circle" file="garden_circle.png"></g:img>
                        <h2>Garden</h2>
                        <p>
                            Your growing space is attached to your property.
                        </p>
                        <g:link class="btn btn-default" controller="garden" action="create" >Get Started</g:link>
                    </div>

                    <g:ifHydro>
                        <div class="col-lg-4">
                            <g:img class="img-circle" file="hydro_circle.png"></g:img>
                            <h2>Hydroponics</h2>
                            <p>
                                Your growing space is based on hydroponics
                            </p>
                            <g:link class="btn btn-default" controller="hydroponics" action="create" >Get Started</g:link>
                        </div>
                        
                        <div class="col-lg-4">
                            <g:img class="img-circle" file="aqua_circle.png"></g:img>
                            <h2>Aquaponics</h2>
                            <p>
                                Your growing space is based on aquaponics
                            </p>
                            <g:link class="btn btn-default" controller="aquaponics" action="create" >Get Started</g:link>
                        </div>
                    </g:ifHydro>

                    <div class="col-lg-4">
                        <g:img class="img-circle" file="other_circle.png"></g:img>
                        <h2>Other</h2>
                        <p>
                            Your growing space is neither a garden nor an allotment. Let us know what it is.
                        </p>
                        <g:link class="btn btn-default" controller="otherGrowingSpace" action="create" >Get Started</g:link>
                    </div>

            </div>
        </div>
    </body>
</html>
