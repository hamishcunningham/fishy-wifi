<%@ page defaultCodec="html" %>
<div class="form-group ${invalid ? 'error' : ''} widget">
    <label for="dateCreated_month">
        Harvest Date:
    </label>

<div class="form-inline">

        <div class="form-group">
        <f:datePickerWClass name="dateHarvested" default="${new Date()}"
                            noSelection="['':'-Choose-']"
                            precision="month"
                            klass="form-control"
                            relativeYears="${[-10..0]}"
                            value="${harvest.dateHarvested}"
        />


        </div>

    <g:if test="${invalid}"><p class="help-block">${errors.join('<br>')}</p></g:if>
</div>
</div>
