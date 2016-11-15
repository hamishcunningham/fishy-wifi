<%@ page defaultCodec="html" %>
<g:if test="${type == Boolean}">
    <div class="checkbox">
        <label>
              <%= widget %> <g:message code="${entityName}.${property}.question" default="${label}" />
        </label>
    </div>
</g:if>
<g:else>
    <div class="form-group ${invalid ? 'error' : ''} widget">
        <label for="${property}">
            <g:message code="${entityName}.${property}.question" default="${label}" />
        </label>

            <%= widget %>
            <g:if test="${invalid}"><p class="help-block">${errors.join('<br>')}</p></g:if>
    </div>
</g:else>
