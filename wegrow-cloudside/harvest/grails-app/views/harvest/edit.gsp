<!DOCTYPE html>
<html>
    <head>
        <meta name="layout" content="small-form" />
        <g:set var="entityName" value="${message(code: 'harvest.label', default: 'Harvest')}" />
        <title><g:message code="default.edit.label" args="[entityName]" /></title>
    </head>
    <body>
        <div id="edit-harvest" class="content scaffold-edit" role="main">
            <h1><g:message code="default.edit.label" args="[entityName]" /></h1>
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
            <g:form resource="${this.harvest}" method="PUT">
                <g:hiddenField name="version" value="${this.harvest?.version}" />
                <fieldset class="form">
                    <f:allWTransients bean="harvest"/>
                </fieldset>
                <div class="checkbox">
                    <label>
                        <g:checkBox name="cropFinished" />
                        <g:message code="harvest.cropFinished.question" default="Crop finished" />
                    </label>
                </div>

                <fieldset class="buttons">
                    <input class="save" type="submit" value="${message(code: 'default.button.update.label', default: 'Update')}" />
                </fieldset>
            </g:form>
        </div>
    </body>
</html>
