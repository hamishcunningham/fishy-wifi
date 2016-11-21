

<!DOCTYPE html>
<html>
    <head>
        <meta name="layout" content="main" />
        <title>Admin Panel</title>
    </head>
    <body>
    <p>
        <g:link class="btn btn-default btn-large" resource="harvest" action="index">
            <g:message default="Manage Harvests" code="harvests.index.label" />
        </g:link>
    </p>

    <p>
        <g:link class="btn btn-default btn-large" resource="report" action="index">
            <g:message default="Get Crop Reports" code="reports.index.label" />
        </g:link>
    </p>

    <p>
        <g:link class="btn btn-default btn-large" resource="crop" action="index">
            <g:message default="Manage Crops" code="crops.index.label" />
        </g:link>
    </p>

    <g:link class="btn btn-default btn-large" resource="user">
        <g:message default="Manage Users" code="users.index.label" />
    </g:link>

<div id="controllers" role="navigation">
                <h2>Available Controllers:</h2>
                <ul>
                    <g:each var="c" in="${grailsApplication.controllerClasses.sort { it.fullName } }">
                        <li class="controller">
                            <g:link controller="${c.logicalPropertyName}">${c.fullName}</g:link>
                        </li>
                    </g:each>
                </ul>
            </div>

</body>
</html>