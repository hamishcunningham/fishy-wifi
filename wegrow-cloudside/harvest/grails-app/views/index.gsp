<!DOCTYPE html>
<html>
    <head>
        <meta name="layout" content="main" />
        <g:set var="entityName" value="${message(code: 'growingSpace.label', default: 'GrowingSpace')}" />
        <title><g:message code="default.create.label" args="[entityName]" /></title>
    </head>
    <body>
        <g:ifHydro>
            Text for hydroponics homepage here.
        </g:ifHydro>
        <g:ifConventional>
            Text for conventional growing here.
        </g:ifConventional>
    </body>
</html>