<%@ page defaultCodec="html" %>

    <div class="form-group ${invalid ? 'error' : ''} widget">
        <label for="${property}">
            <g:message code="${controllerName}.${property}.question" default="${label}" />
        </label>

            <f:widget bean="${bean}" property="${property}" from="${allowedAreas}" value="${value}"/>
            <g:if test="${invalid}"><p class="help-block">${errors.join('<br>')}</p></g:if>
    </div>
