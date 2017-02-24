<!DOCTYPE html>
<html>
    <head>
        <meta name="layout" content="small-form" />
        <g:set var="entityName" value="${message(code: 'harvest.label', default: 'Harvest')}" />
        <title><g:message code="default.create.label" args="[entityName]" /></title>
    </head>
    <body>
        <div id="create-harvest" class="content scaffold-create" role="main">
            <h1><g:message code="default.create.label" args="[entityName]" /></h1>
            <g:if test="${flash.message}">
            <div class="message" role="status">${flash.message}</div>
            </g:if>
            <g:hasErrors bean="${this.harvest}">
            <ul class="errors" role="alert">
                <g:eachError bean="${this.harvest}" var="error">
                <li <g:if test="${error in org.springframework.validation.FieldError}">data-field-id="${error.field}"</g:if>><g:message error="${error}"/></li>
                </g:eachError>
            </ul>
            </g:hasErrors>
            <g:form action="save">
                <fieldset class="form">
                    <f:field bean="harvest" property="area" value="${selectedArea}" />
                    <f:field bean="harvest" property="weight"/>

                    <div class="checkbox">
                        <label>
                              <g:checkBox name="cropFinished" />
                              <g:message code="harvest.cropFinished.question" default="Crop finished" />
                        </label>
                    </div>

                </fieldset>
                <fieldset class="buttons">
                    <g:submitButton name="create" class="save btn btn-primary btn-lg" value="${message(code: 'default.button.create.label', default: 'Create')}" />
                </fieldset>
            </g:form>
        </div>
    </body>
</html>
