<!DOCTYPE html>
<html>
    <head>
        <meta name="layout" content="main" />
        <g:set var="entityName" value="${message(code: 'hydroponics.label', default: 'Hydroponics')}" />
        <title><g:message code="default.list.label" args="[entityName]" /></title>
    </head>
    <body>
        <div id="list-hydroponics" class="content scaffold-list" role="main">
            <h1><g:message code="default.list.label" args="[entityName]" /></h1>
            <g:if test="${flash.message}">
                <div class="message" role="status">${flash.message}</div>
            </g:if>
            <f:table collection="${hydroponicsList}" />

            <div class="pagination">
                <g:paginate total="${hydroponicsCount ?: 0}" />
            </div>

            <g:link class="create" action="create" class="btn btn-primary"><g:message code="default.new.label" args="[entityName]" /></g:link>
        </div>
    </body>
</html>