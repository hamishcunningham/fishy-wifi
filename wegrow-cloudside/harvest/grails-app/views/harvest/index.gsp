<!DOCTYPE html>
<html>
<head>
    <meta name="layout" content="main"/>
    <g:set var="entityName" value="${message(code: 'harvest.label', default: 'Harvest')}"/>
    <title><g:message code="harvest.title"/></title>
</head>

<body>
<div id="list-harvest" class="content scaffold-list" role="main">
    <h1><g:message code="harvest.title"/></h1>
    <g:if test="${flash.message}">
        <div class="message" role="status">${flash.message}</div>
    </g:if>
    <g:if test="${harvestList}">

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
                    <td><g:formatDate date="${harvest.logged_at}" type="datetime" style="SHORT" locale="uk"/></td>
                    <td><g:formatNumber number="${harvest.area_m2}" type="number" maxFractionDigits="2" />m²</td>
                    <td>${harvest.radius}<g:if test="${harvest.radius}">m</g:if></td>
                    <td>${harvest.crop}</td>
                    <td><g:formatNumber number="${harvest.weight_g}" type="number" maxFractionDigits="2" />g</td>
                    <td><g:formatNumber number="${harvest.yield_m2}" type="number" maxFractionDigits="2" />g/m²</td>
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
        <export:formats formats="['csv', 'excel', 'xml']"></export:formats>



        <div class="text-center"><g:link class="create" action="create" class="btn btn-lg btn-primary">
            <g:message code="harvest.create.title" /></g:link></div>

        <div>

    </g:if>
    <g:else>
        <div class="text-center"><p class="lead"><g:message code="harvest.empty" default="You have not logged any harvests yet." /></p></div>
        <div class="text-center"><g:link controller="harvest" action="create" class="btn btn-lg btn-primary">
            <g:message code="harvest.empty.button" default="Get started." /></g:link></div>
    </g:else>

    </div>

</div>
</body>
</html>