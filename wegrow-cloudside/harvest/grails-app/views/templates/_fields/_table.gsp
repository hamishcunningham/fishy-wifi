<table class="table .table-striped">
    <thead>
         <tr>
            <f:eachProperty domainClass="${domainClass}" var="p">
                <g:set var="propTitle">${domainClass.propertyName}.${p.name}.label</g:set>
                <g:sortableColumn property="${p.name}" title="${message(code: propTitle, default: p.naturalName)}" />
            </f:eachProperty>
        </tr>
    </thead>
    <tbody>
        <g:each in="${collection}" var="bean" status="i">
            <tr>
                <f:eachProperty domainClass="${domainClass}" var="p" status="j">
                    <g:if test="${j==0}">
                        <td><g:link method="GET" resource="${bean}">
                            ${bean[p.name]}
                         </g:link></td>
                    </g:if>
                    <g:else>
                        <td><f:display bean="${bean}" property="${p.name}"  displayStyle="${displayStyle?:'table'}" /></td>
                    </g:else>
                </f:eachProperty>
            </tr>
        </g:each>
    </tbody>
</table>