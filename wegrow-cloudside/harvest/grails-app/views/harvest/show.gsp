<!DOCTYPE html>
<html>
    <head>
        <meta name="layout" content="main" />
        <g:set var="entityName" value="${message(code: 'harvest.label', default: 'Harvest')}" />
        <title><g:message code="default.show.label" args="[entityName]" /></title>
    </head>
    <body>
        <div id="show-harvest" class="content scaffold-show" role="main">
            <h1><g:message code="default.show.label" args="[entityName]" /></h1>
            <g:if test="${flash.message}">
            <div class="message" role="status">${flash.message}</div>
            </g:if>
            <f:display bean="harvest" />
            <g:form resource="${this.harvest}" method="DELETE">
                <fieldset class="buttons">
                    <g:link class="edit" action="edit" resource="${this.harvest}"><g:message code="default.button.edit.label" default="Edit" /></g:link>
                    <input class="delete" type="submit" value="${message(code: 'default.button.delete.label', default: 'Delete')}" onclick="return confirm('${message(code: 'default.button.delete.confirm.message', default: 'Are you sure?')}');" />
                </fieldset>
            </g:form>
        </div>
    </body>
</html>
