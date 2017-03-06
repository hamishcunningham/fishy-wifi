<html>
<head>
    <meta name="layout" content="12col"/>
    <title><g:message code="spring.security.ui.register.title"/></title>
</head>
<body>

<div id="login">
        <div class="row">
            <h1 class="text-center"><g:message code='spring.security.ui.register.title'/></h1>
            <g:if test='${emailSent}'>
                <div class="alert col-md-6 col-md-offset-3 text-center">
                    <g:message code='spring.security.ui.register.sent'/>
                </div>
            </g:if>
        </div>

        <div class="col-md-4 col-md-offset-4">

            <g:if test='${flash.message}'>
                <div class="login_message alert">${flash.message}</div>
            </g:if>


            <g:if test='${!emailSent}'>
                <form action="${postUrl ?: '/register/register'}" method="POST" id="loginForm" class="form-horizontal" autocomplete="off">
                    <fieldset class="form">

                        <f:field bean="registerCommand" property="username"/>
                        <f:field bean="registerCommand" property="email"/>
                        <f:field bean="registerCommand" widget-type="password" property="password"/>
                        <f:field bean="registerCommand" widget-type="password" property="password2"/>
                    </fieldset>
                    <fieldset class="text-center">
                        <g:submitButton name="create" class="save btn btn-primary" value="${message(code: 'spring.security.ui.register.submit', default: 'Register')}" />
                    </fieldset>

                </form>
            </g:if>
        </div>
</div>
<script>
    (function() {
        document.forms['loginForm'].elements['${usernameParameter ?: 'username'}'].focus();
    })();
</script>

</body>
</html>