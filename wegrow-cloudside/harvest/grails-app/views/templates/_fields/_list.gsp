<dl class="dl-horizontal ${domainClass.propertyName}">
    <f:eachProperty domainClass="${domainClass}" var="p">
        <dt class="fieldcontain">
            <span id="${p.name}-label" class="property-label"><g:message code="${domainClass.propertyName}.${p.name}.label" default="${p.naturalName}" /></span>
        </dt>
        <dd>
            <div class="property-value" aria-labelledby="${p.name}-label">${body(p)}</div>
        </dd>
    </f:eachProperty>
</ol>
