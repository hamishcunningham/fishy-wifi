<%@ page defaultCodec="html" %>
<div class="form-group ${invalid ? 'error' : ''} widget">
    <label>
        <g:message code="${controllerName}.${property}.question" default="${label}"/>
    </label>

    <div class="form-group row">
        <div class="col-md-6">
            <label for="yearsGrowing">
                Years:
            </label>
            <f:widget bean="bean" property="yearsGrowing"/>
        </div>

        <div class="col-md-6">
            <label for="monthsGrowing">
                Months:
            </label>
            <f:widget bean="bean" property="monthsGrowing"/>
        </div>
        <g:if test="${invalid}"><p class="help-block">${errors.join('<br>')}</p></g:if>
    </div>
    </div>
</div>
