<!DOCTYPE html>
<html>
<head>
    <meta name="layout" content="main" />
    <g:set var="entityName" value="${message(code: 'user.label', default: 'User')}" />
    <title><g:message code="default.list.label" args="[entityName]" /></title>
</head>
<body>
<div id="list-area" class="content scaffold-list" role="main">
    <h1><g:message code="default.list.label" args="[entityName]" /></h1>
    <g:if test="${flash.message}">
        <div class="message" role="status">${flash.message}</div>
    </g:if>
    <table class="table table-striped">
        <tr>
            <th>Username</th>
            <th>Email</th>
            <th>Organic</th>
            <th>Signature</th>
            <th>Years growing</th>
            <th>Months growing</th>
            <th>All data?</th>
            <th>Location</th>
            <th></th>
        </tr>
        <g:each var="user" in="${userList}" >
            <tr>
                <td>
                    ${user.username}
                </td>
                <td>
                    ${user.email}
                </td>
                <td>
                    ${user.growingSpace?.isOrganic}
                </td>
                <td>
                    ${user.growingSpace?.electronicSignature}
                </td>

                <td>
                    ${user.growingSpace?.yearsGrowing}
                </td>

                <td>
                    ${user.growingSpace?.monthsGrowing}
                </td>
                <td>
                    ${user.growingSpace?.submittingAllData}
                </td>
                <td>
                    ${user.growingSpace?.locationString}
                </td>
                <td>
                <g:form controller="user" class="form-inline controls-form" action="delete" id="${user.id}" method="DELETE">
                    <input class="delete btn btn-danger"
                           type="submit" value="${message(code: 'default.button.delete.label', default: 'Delete')}"
                           onclick="return confirm('${message(code: 'default.button.delete.confirm.message', default: 'Are you sure?')}');"/>
                </g:form>

                </td>
            </tr>
        </g:each>

    </table>

    <div class="pagination">
        <g:paginate total="${userCount ?: 0}" />
    </div>

    <export:formats formats="['csv', 'excel', 'xml']"></export:formats>

</div>
</body>
</html>