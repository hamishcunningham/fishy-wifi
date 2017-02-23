<!DOCTYPE html>
<html>
    <head>
        <meta name="layout" content="main" />
        <g:set var="entityName" value="${message(code: 'area.label', default: 'Area')}" />
        <title><g:message code="default.edit.label" args="[entityName]" /></title>
    </head>
    <body>


        <div id="edit-preferences" class="content scaffold-edit" role="main">
            <h1><g:message code="user.preferences.title" default="User Preferences" /></h1>
            <p class="lead">
                These are the units displayed when you look at your harvest record in Manage Harvests.
                You will still be able to enter new growing areas and harvest weights in either unit
            </p>
            <g:if test="${flash.message}">
            <div class="message" role="status">${flash.message}</div>
            </g:if>
            <g:hasErrors bean="${this.area}">
            <ul class="errors" role="alert">
                <g:eachError bean="${this.area}" var="error">
                <li <g:if test="${error in org.springframework.validation.FieldError}">data-field-id="${error.field}"</g:if>><g:message error="${error}"/></li>
                </g:eachError>
            </ul>
            </g:hasErrors>
            <g:form action="updatePreferences" method="POST">
                <fieldset class="form">
                <g:each var="pref" in="${preferences}">
                    <f:field bean="${user}" property="${pref}" />
                </g:each>
                </fieldset>
                <fieldset class="buttons">
                    <input class="save" type="submit" value="${message(code: 'default.button.update.label', default: 'Update')}" />
                </fieldset>
            </g:form>
        </div>
    </body>
</html>
