<!DOCTYPE html>
<html>
<head>
    <link rel="stylesheet" type="text/css" href="grails-app/assets/stylesheets/application_conventional.less"/><meta name="layout" content="main"/>
    <g:set var="entityName" value="${message(code: 'harvest.label', default: 'Harvest')}"/>
    <title><g:message code="harvest.title"/></title>
</head>

<body>
<div id="list-harvest" class="content scaffold-list" role="main">
    <h1><g:message code="harvest.title"/></h1>

    <g:if test="${flash.message}">
        <div class="message" role="status">${flash.message}</div>
    </g:if>
    <g:if test="${areaList}">
        <table class="table">
            <tr>
                <th >Crop</th>
                <sec:access expression="hasRole('ROLE_ADMIN')">
                    <th class="text-center">Email address</th><th class="text-center">Growing Space</th>
                </sec:access>
                <th class="text-center">Area/Radius</th>
                <th class="text-center">Amount</th>
                <th class="text-center">Yield</th>
                <th class="text-center">Date</th>
                <th />
        </tr>

            <g:each var="area" in="${areaList}">
                <tr class="harvestHeadingRow">
                    <td>${area.crop}</td>

                    <sec:access expression="hasRole('ROLE_ADMIN')">
                        <td class="text-center">${area.space.user.email}</td>
                        <td class="text-center">${area.crop.type}</td>
                    </sec:access>

                    <td class="text-center">
                        <g:if test="${area.crop.isTree}">
                            <g:formatNumberUnit number="${area.canopyRadius}" type="radius" maxFractionDigits="2" /> (canopy)
                        </g:if>
                        <g:else>
                            <g:formatNumberUnit number="${area.areaMeters}" type="area" maxFractionDigits="2" />
                        </g:else>
                    </td>

                    <td class="text-center">
                        <g:formatNumberUnit number="${area.amountGrammes}" type="weight" maxFractionDigits="2" />
                    </td>
                    <td class="text-center"><g:formatNumberUnit number="${area.yield}" type="yield" maxFractionDigits="2" /></td>

                    <td />
                    <td class="text-right">
                        <g:link class="btn btn-success" controller="harvest" action="create" params="${[areaId: area.id]}">Log a harvest</g:link>
                    </td>

                </tr>

                <g:each var="harvest" in="${area.harvests}">
                    <tr>
                        <td />
                        <td />

                        <sec:access expression="hasRole('ROLE_ADMIN')">
                            <td />
                            <td />
                        </sec:access>
                        <td class="text-center"><g:formatNumberUnit number="${harvest.weightGrammes}" type="weight" maxFractionDigits="2" /></td>
                        <td class="text-center"><g:formatNumberUnit number="${harvest.yield}" type="yield" maxFractionDigits="2" /></td>
                        <td class="text-center">
                            <g:formatDate date="${harvest.dateCreated}"
                                      format="EEEE, d MMMM yyyy" locale="gb"/>
                        </td>
                        <td class="text-right">

                            <g:form controller="harvest" action="delete" id="${harvest.id}" class="inline" method="DELETE">
                                <input class="delete btn btn-danger"
                                       type="submit" value="${message(code: 'default.button.delete.label', default: 'Delete')}"
                                       onclick="return confirm('${message(code: 'default.button.delete.confmlirm.message', default: 'Are you sure?')}');"/>
                            </g:form>
                            <g:link class="btn btn-primary" action="edit" id="${harvest.id}">Edit</g:link>

                        </td>

                    </tr>
                </g:each>
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