<%@ page defaultCodec="html" %>
<div class="form-group ${invalid ? 'error' : ''}">
    <label for="${property}">
        <g:message code="${controllerName}.${property}.question" default="${label}" />
    </label>

    <div class="input-group">
    <div class="widget"><%= widget %></div>
      <span class="input-group-addon">

        <label>
            <input type="radio" aria-label="meters" value="SQ_METERS" name="unit" /> square meters
        </label>

        <label>
            <input type="radio" aria-label="feet" value="SQ_FEET" name="unit" / /> square feet
        </label>
      </span>

    </div><!-- /input-group -->
          <g:if test="${invalid}"><p class="help-block">${errors.join('<br>')}</p></g:if>
</div>
