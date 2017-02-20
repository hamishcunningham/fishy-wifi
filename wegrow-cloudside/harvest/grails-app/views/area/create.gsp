<!DOCTYPE html>
<html>
    <head>
        <meta name="layout" content="main" />
        <g:set var="entityName" value="${message(code: 'area.label', default: 'Area')}" />
        <title><g:message code="default.create.label" args="[entityName]" /></title>
    </head>
    <body>
        <div id="create-area" class="content scaffold-create" role="main">

            <h1><g:message code="area.title" /></h1>
            <p><g:message code="area.question" /></p>

            <div id="list-area">
                <g:if test="${areaList}">
                    <table class="table table-striped">
                    <tr>
                        <th>Crop</th>
                        <th class="text-center">Area/Radius</th>
                        <th class="text-center" >Indoors/Outdoors?</th>
                        <th class="text-center">Finished</th>
                        <th></th>
                    </tr>
                <g:each var="area" in="${areaList}">
                    <tr>
                        <td>${area.crop}</td>
                        <td class="text-center">
                            <g:if test="${area.crop.isTree}">
                                <g:formatNumberUnit number="${area.canopyRadius}" type="radius" maxFractionDigits="2" />
                            </g:if>
                            <g:else>
                                <g:formatNumberUnit number="${area.areaMeters}" type="area" maxFractionDigits="2" />
                            </g:else>
                        </td>
                        <td class="text-center">
                            ${area.inGreenhouse ? "In Greenhouse" : "Outdoors"}
                        </td>
                        <td class="text-center">
                            ${area.finished ? "Finished" : "Not finished"}
                        </td>
                        <td class="text-right">
                            <g:form controller="area" class="form-inline controls-form" action="delete" id="${area.id}" method="DELETE">
                                <input class="delete btn btn-danger"
                                       type="submit" value="${message(code: 'default.button.delete.label', default: 'Delete')}"
                                       onclick="return confirm('${message(code: 'default.button.delete.confirm.message', default: 'Are you sure?')}');"/>
                            </g:form>
                            <g:link class="btn btn-primary" action="edit" id="${area.id}">Edit</g:link>
                            <g:form controller="area" class="form-inline controls-form" action="update" id="${area.id}" method="PUT">
                                <g:if test="${area.finished != true}">
                                    <input type="hidden" name="finished" value="true" />
                                    <input class="delete btn btn-success"
                                           type="submit" value="${message(code: 'area.button.finished', default: 'Done Harvesting')}"/>
                                </g:if>
                                <g:else>
                                    <input type="hidden" name="finished" value="false" />

                                    %{--<input class="delete btn btn-primary"--}%
                                           %{--type="submit" value="${message(code: 'area.button.finished', default: 'Continue Harvesting')}"/>--}%

                                </g:else>

                            </g:form>

                            <g:if test="${area.finished != true}">
                                <g:link class="btn btn-primary" controller="harvest" action="create" params="${[areaId: area.id]}">Log a harvest</g:link>
                            </g:if>
                        </td>
                    </tr>
                </g:each>
                </table>
                </g:if>

            </div>

            <g:if test="${flash.message}">
                <div class="message alert" role="status">${flash.message}</div>
            </g:if>
            <g:hasErrors bean="${this.area}">
            <ul class="errors" role="alert">
                <g:eachError bean="${this.area}" var="error">
                    <li <g:if test="${error in org.springframework.validation.FieldError}">data-field-id="${error.field}"</g:if>><g:message error="${error}"/></li>
                </g:eachError>
            </ul>
            </g:hasErrors>
            <g:form action="save">
                <fieldset class="form">
                    <f:field bean="area" property="crop"/>
                    <f:field bean="area" property="area"/>
                    <f:field bean="area" property="canopyRadius"/>
                    <f:field bean="area" property="inGreenhouse"/>
                </fieldset>

                <fieldset class="buttons">
                    <g:submitButton name="create" class="save btn btn-primary" value="${message(code: 'default.button.create.label', default: 'Create')}" />
                </fieldset>
            </g:form>
        </div>
    </body>
</html>
