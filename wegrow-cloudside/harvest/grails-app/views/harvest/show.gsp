<!DOCTYPE html>
<html>
    <head>
        <meta name="layout" content="main" />
        <g:set var="entityName" value="${message(code: 'harvest.label', default: 'Harvest')}" />
        <title><g:message code="default.show.label" args="[entityName]" /></title>
    </head>
    <body>
        <div id="show-harvest" class="content scaffold-show" role="main">
            <h1>Harvest updated</h1>
            <g:if test="${flash.message}">
            <div class="message" role="status">${flash.message}</div>
            </g:if>
            <f:display bean="harvest" />
            <g:form resource="${this.harvest}" method="DELETE">
                <fieldset class="buttons">
                    <g:link class="edit btn btn-primary" action="edit" resource="${this.harvest}"><g:message code="default.button.edit.label" default="Edit" /></g:link>
                    <input class="delete btn btn-danger" type="submit" value="${message(code: 'default.button.delete.label', default: 'Delete')}" onclick="return confirm('${message(code: 'default.button.delete.confirm.message', default: 'Are you sure?')}');" />
                    <g:link class="edit btn btn-primary" action="index" resource="${this.harvest}">Manage Harvests</g:link>
                </fieldset>
            </g:form>
        </div>
    </body>
</html>
