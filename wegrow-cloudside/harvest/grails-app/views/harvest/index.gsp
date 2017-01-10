<!DOCTYPE html>
<html>
<head>
    <meta name="layout" content="main"/>
    <g:set var="entityName" value="${message(code: 'harvest.label', default: 'Harvest')}"/>
    <title><g:message code="default.list.label" args="[entityName]"/></title>
</head>

<body>
<div id="list-harvest" class="content scaffold-list" role="main">
    <h1><g:message code="default.list.label" args="[entityName]"/></h1>
    <g:if test="${flash.message}">
        <div class="message" role="status">${flash.message}</div>
    </g:if>
    <table class="table table-striped">
        <tr>
            <sec:access expression="hasRole('ROLE_ADMIN')">
                <th>Email address</th><th>Growing Space</th>
            </sec:access>
            <th>Date</th><th>Area</th><th>Radius</th><th>Crop</th><th>Amount</th><th>Yield</th><th/><th/>
        </tr>

        <g:each var="harvest" in="${harvestList}">
            <tr>
                <sec:access expression="hasRole('ROLE_ADMIN')">
                    <td>${harvest.email}</td>
                    <td>${harvest.type}</td>
                </sec:access>
                <td>${harvest.logged_at}</td>
                <td>${harvest.area_m2}</td>
                <td>${harvest.radius}</td>
                <td>${harvest.crop}</td>
                <td>${harvest.weight_g}g</td>
                <td>${harvest.yield_m2}g/m2</td>
                <td>

                    <g:form controller="harvest" action="delete" id="${harvest.id}" method="DELETE">
                        <input class="delete btn btn-danger"
                               type="submit" value="${message(code: 'default.button.delete.label', default: 'Delete')}"
                               onclick="return confirm('${message(code: 'default.button.delete.confirm.message', default: 'Are you sure?')}');"/>
                    </g:form>
                </td>
                <td>
                    <g:link class="btn btn-primary" action="edit" id="${harvest.id}">Edit</g:link>

                </td>
            </tr>
        </g:each>
    </table>

    <div class="paginate">
        <g:paginate total="${harvestCount ?: 0}"/>
    </div>

    <div><g:link class="create" action="create" class="btn btn-primary">
        <g:message code="default.new.label"
                   args="[entityName]"/></g:link></div>
    <div >
    <export:formats formats="['csv', 'excel', 'xml']"/>

    </div>

</div>
</body>
</html>