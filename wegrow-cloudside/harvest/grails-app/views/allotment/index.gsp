<!DOCTYPE html>
<html>
    <head>
        <meta name="layout" content="main" />
        <g:set var="entityName" value="${message(code: 'allotment.label', default: 'Allotment')}" />
        <title><g:message code="default.list.label" args="[entityName]" /></title>
    </head>
    <body>
        <div id="list-allotment" class="content scaffold-list" role="main">
            <h1><g:message code="default.list.label" args="[entityName]" /></h1>
            <g:if test="${flash.message}">
                <div class="message" role="status">${flash.message}</div>
            </g:if>
            <f:table collection="${allotmentList}" />

            <div class="pagination">
                <g:paginate total="${allotmentCount ?: 0}" />
            </div>

            <g:link class="create" action="create" class="btn btn-primary"><g:message code="default.new.label" args="[entityName]" /></g:link>
        </div>
    </body>
</html>