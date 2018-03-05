<!DOCTYPE html>
<html>
    <head>
        <link rel="stylesheet" type="text/css"
              href="grails-app/assets/stylesheets/application_conventional.less"/><meta name="layout" content="main" />
        <g:set var="entityName" value="${message(code: 'crop.label', default: 'Crop')}" />
        <title><g:message code="default.list.label" args="[entityName]" /></title>
    </head>
    <body>
        <div id="list-crop" class="content scaffold-list" role="main">
        <div class="pull-right">
                <g:link class="create" action="create" class="btn btn-primary btn-lg"><g:message
                    code="crop.new" args="[entityName]"/></g:link>
            </div>
            <h1><g:message code="default.list.label" args="[entityName]" /></h1>


            <p class="subTitle">
                These are the crops which are available to users of the system. It's not possible to log crops
                which are not listed here.</p>
                
                


            <g:if test="${flash.message}">
                <div class="message" role="status">${flash.message}</div>
            </g:if>

        <table class="table table-striped">
        <tr>
            <th>Type</th><th class="text-center">Is Tree?</th><th class="text-center">Can be grown indoors?</th>
            <th class="text-center">Areas</th><th class="text-center">Harvests</th>
            <th class="text-center">Average Yield</th><th/>
        </tr>

        <g:each var="crop" in="${cropList}">
            <tr>
                <td>${crop.type}</td>
                <td class="text-center">${crop.isTree ? "Tree" : "Other" }</td>
                <td class="text-center">${crop.isGreenhouseable ? "Indoors & Outdoors" : "Outdoors only"}</td>
                <td class="text-center">${crop.areas.size()}</td>
                <td class="text-center">${crop.harvests.size()}</td>
                <td class="text-center"><g:formatNumber number="${crop.yield}" type="number" maxFractionDigits="2" />g/m²</td>

                <td class="text-right">
                    <g:link class="btn btn-primary" action="edit" id="${crop.id}">Edit</g:link>
                    <g:form controller="crop" action="delete" id="${crop.id}" method="DELETE" class="inline">
                        <input class="btn btn-danger"
                               type="submit" value="${message(code: 'default.button.delete.label', default: 'Delete')}"
                               onclick="return confirm('${message(code: 'default.button.delete.confirm.message', default: 'Are you sure?')}');"/>
                    </g:form>

                </td>

            </tr>
        </g:each>

        </table>
        <div class="paginate">
            <g:paginate total="${cropCount ?: 0}" />
            <g:form class="inline" controller="crop" action="index">Items per page: <g:select onChange="submit();" name="max" from="${[10,50,100,200]}" value="${params.max}" /></g:form>
        </div>
            
            <export:formats formats="['csv', 'excel', 'xml']"></export:formats>

        </div>
    </body>
</html>