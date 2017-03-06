<%@ page import="grails.plugin.springsecurity.SpringSecurityUtils" %>
<!DOCTYPE html>
<html>
<head>
    <link rel="stylesheet" type="text/css" href="grails-app/assets/stylesheets/application_conventional.less"/><link rel="stylesheet" type="text/css" href="grails-app/assets/stylesheets/application_conventional.less"/><link rel="stylesheet" type="text/css" href="grails-app/assets/stylesheets/application_conventional.less"/><link rel="stylesheet" type="text/css" href="grails-app/assets/stylesheets/application_conventional.less"/><meta name="layout" content="main"/>
    <g:set var="entityName" value="${message(code: 'harvest.label', default: 'Harvest')}"/>
    <title><g:message code="harvest.title"/></title>
</head>

<body>
<div id="list-harvest" class="content scaffold-list" role="main">
    <h1><g:message code="harvest.title"/></h1>
    <%
        def colCount = grails.plugin.springsecurity.SpringSecurityUtils.ifAllGranted('ROLE_ADMIN') ? 12: 9
    %>
    <g:if test="${flash.message}">
        <div class="message" role="status">${flash.message}</div>
    </g:if>
    <g:if test="${areaList}">
        <table class="table stacktable">
            <tr>
                <th >Name</th>

                <th >Crop</th>
                <th >Variety</th>

                <sec:access expression="hasRole('ROLE_ADMIN')">
                    <th class="text-center">Email address</th><th class="text-center">Location</th>
                </sec:access>
                <th class="text-center">Area/Radius</th>
                <th class="text-center">Amount</th>
                <th class="text-center">Yield</th>
                <th></th>
        </tr>

            <div class="well-sm">
                <h3>Instructions:</h3>
                <p>
                    The following table lists the crops that you've told us you're growing. Click the icons in the table
                    to record your harvests.
                </p>
                <ul class="icon-key">
                    <li>
                        <span class="text-danger glyphicon glyphicon-trash"></span> Delete
                    </li>

                    <li>
                        <span class="glyphicon glyphicon-pencil"></span> Edit
                    </li>


                    <li>
                        <span class="glyphicon glyphicon-ok"></span> Mark as "finished for the year"
                    </li>


                    <li>
                        <span class="myharvest-orange glyphicon glyphicon-chevron-right"></span> Show harvests for this crop
                    </li>


                </ul>
            </div>
            <g:each var="area" in="${areaList}" >
                <tr class="harvestHeadingRow ${area.finished?'finished':''}" data-toggle="collapse" data-target=".rows-${area.id}">
                    <td >${area.toString()}</td>
                    <td >${area.crop}</td>
                    <td >${area.variety}</td>

                    <sec:access expression="hasRole('ROLE_ADMIN')">
                        <td class="text-center">${area.space.user.email}</td>
                        <td class="text-center">${area.space.locationString}</td>
                    </sec:access>

                    <td class="text-center">
                        <g:if test="${area.crop.isTree}">
                            <g:formatNumberUnit number="${area.canopyRadiusMeters}" type="radius" maxFractionDigits="2" /> (canopy)
                        </g:if>
                        <g:else>
                            <g:formatNumberUnit number="${area.areaMeters}" type="area" maxFractionDigits="2" />
                        </g:else>
                    </td>

                    <td class="text-center">
                        <g:formatNumberUnit number="${area.amountGrammes}" type="weight" maxFractionDigits="2" />
                    </td>
                    <td class="text-center"><g:formatNumberUnit number="${area.yield}" type="yield" maxFractionDigits="2" /></td>

                    <td class="text-right">
                        <g:if test="${area.finished}">
                                <span class="label harvest-green-label" aria-hidden="true"
                                      data-toggle="tooltip" title="You've recorded the last harvest for this crop">
                                    HARVEST COMPLETE
                                </span>


                        </g:if>


                        <g:form controller="area" action="delete" id="${area.id}" class="inline" method="DELETE">
                        <button class="delete btn btn-link"
                               type="submit"
                                data-toggle="tooltip"
                                        title="${message(code: 'default.button.delete.label', default: 'Delete')}"
                                onclick="return confirm('${message(code: 'default.button.delete.confmlirm.message', default: 'Are you sure?')}');">

                            <span class="glyphicon glyphicon-trash text-danger" aria-hidden="true"></span><span class="sr-only">Delete</span>
                        </button>
                        </g:form>
                        <g:link class="btn btn-link" controller="area" action="edit" id="${area.id}"
                            data-toggle="tooltip"
                                title="${message(code: 'default.button.edit.label', default: 'Edit')}">
                            <span class="glyphicon glyphicon-pencil" aria-hidden="true"></span><span class="sr-only">Edit</span>
                        </g:link>

                        <g:form controller="area" class="form-inline controls-form" action="update" id="${area.id}" method="PUT">
                            <g:if test="${area.finished != true}">
                                <input type="hidden" name="finished" value="true" />
                                <button class="delete btn btn-link"
                                       type="submit" data-toggle="tooltip"
                                        title="${message(code: 'area.button.finished', default: 'Mark as Done')}">
                                    <span class="glyphicon glyphicon-ok" aria-hidden="true"></span>
                                    <span class="sr-only">${message(code: 'area.button.finished', default: 'Mark as Done')}</span>


                                </button>
                            </g:if>
                        </g:form>
                        <g:if test="${area.harvests}">
                        <a class="btn btn-link harvest-count myharvest-orange" data-toggle="collapse" data-target=".rows-${area.id}">
                            %{--<span class="harvest-count-num">${area.harvests.size()}</span>--}%
                            <span>${area.harvests.size()}</span>
                            <span data-toggle="tooltip" title="Show or hide Harvests"
                                  class="glyphicon glyphicon-chevron-right" aria-hidden="true"></span>
                            <span class="sr-only">Show/Hide Harvests</span>
                        </a>
                        </g:if>

                        <g:if test="${area.finished != true}">
                            <g:link class="btn btn-harvest-orange myharvest-orange" controller="harvest" action="create" params="${[areaId: area.id]}">
                                <g:img file="ICONS-BLOCK/ICONS-BLOCK-WEIGH-SMALL.png" alt="Add a Harvest" data-toggle="tooltip" title="Add a Harvest"></g:img>
                                <span>Add Harvest</span>
                            </g:link>
                        </g:if>
                    </td>

                    %{--</td>--}%
                    %{--<td>--}%
                        %{--<button type="button" class="btn btn-default" data-toggle="collapse" data-target=".rows-${area.id}">--}%
                        %{--<span class="glyphicon glyphicon-menu-down"></span>--}%
                        %{--</button>--}%

                    %{--</td>--}%

                </tr>

                <g:each var="harvest" in="${area.harvests.sort {it.dateCreated}}">
                    <tr class="harvest-row rows-${area.id}">
                        <td colspan="${colCount}">
                            <div class="row">
                            <div class="col-md-4">
                                <div class="media">
                                    <div class="media-left">
                                        <g:img file="ICONS-STANDARD/ICONS-STANDARD-DATE.png"></g:img>
                                    </div>

                                    <div class="media-body">
                                        <h4>Date</h4>

                                        <span>
                                            <g:formatDate date="${harvest.dateCreated}"
                                                          format="d MMM yyyy" locale="gb"/>
                                        </span>
                                    </div>
                                </div>
                            </div>

                            <div class="col-md-4">
                                <div class="media">
                                    <div class="media-left">
                                        <g:img file="ICONS-STANDARD/ICONS-STANDARD-WEIGH-SMALL.png"></g:img>
                                    </div>

                                    <div class="media-body">
                                        <h4>Weight</h4>

                                        <span><g:formatNumberUnit number="${harvest.weightGrammes}" type="weight"
                                                                  maxFractionDigits="2"/></span>
                                    </div>
                                </div>
                            </div>


                            <div class="col-md-4">
                                <div class="media">
                                    <div class="media-left">
                                        <g:img file="ICONS-STANDARD/ICONS-STANDARD-MARROW-SMALL.png"></g:img>
                                    </div>

                                    <div class="media-body">
                                        <h4>Yield</h4>

                                        <span>
                                            <g:formatNumberUnit number="${harvest.yield}" type="yield" maxFractionDigits="2" />
                                        </span>
                                    </div>
                                </div>
                            </div>
                            </div>
                            <div class="row">
                            <div class="harvestControls">
                                <g:form controller="harvest" action="delete" id="${harvest.id}" class="inline" method="DELETE">
                                    <button class="delete btn btn-link"
                                            type="submit"
                                            data-toggle="tooltip"
                                            title="${message(code: 'default.button.delete.label', default: 'Delete')}"
                                            onclick="return confirm('${message(code: 'default.button.delete.confmlirm.message', default: 'Are you sure?')}');">

                                        <span class="glyphicon glyphicon-trash text-danger" aria-hidden="true"></span><span class="sr-only">Delete</span>
                                    </button>
                                </g:form>
                                <g:link class="btn btn-link" controller="harvest" action="edit" id="${harvest.id}"
                                        data-toggle="tooltip"
                                        title="${message(code: 'default.button.edit.label', default: 'Edit')}">
                                    <span class="glyphicon glyphicon-pencil" aria-hidden="true"></span><span class="sr-only">Edit</span>
                                </g:link>
                            </div>
                            </div>
                        </td>

                    </tr>
                </g:each>
            </g:each>

        </table>
        <div class="paginate">
            <g:paginate total="${harvestCount ?: 0}"/>
        </div>



        <div class="text-center">
            <p>
        <g:link class="create" controller="area" action="create" class="btn btn-lg btn-primary">
            <span class="glyphicon glyphicon-plus"></span> <g:message code="area.create.title" /></g:link></p>
        <div>

        <p class="lead">
            Download your harvest record:
            <g:link action="index" params="${[f:'csv', extension:'csv']}">CSV</g:link>
            <g:link action="index" params="${[f:'excel', extension:'xls']}">Excel</g:link>
            <g:link action="index" params="${[f:'xml', extension:'xml']}">XML</g:link>
        </p>


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