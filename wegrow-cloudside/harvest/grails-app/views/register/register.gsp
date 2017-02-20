<html>
<head>
    <meta name="layout" content="main"/>
    <title><g:message code="spring.security.ui.register.title"/></title>
</head>
<body>

<div id="login">
    <div class="container">
        <div class="col-md-8 col-md-offset-2">
            <h1><g:message code='spring.security.ui.register.title'/></h1>

            <g:if test='${flash.message}'>
                <div class="login_message alert">${flash.message}</div>
            </g:if>

            <g:if test='${emailSent}'>
                <div class="alert">
                    <g:message code='spring.security.ui.register.sent'/>
                </div>
            </g:if>


            <form action="${postUrl ?: '/register/register'}" method="POST" id="loginForm" class="form-horizontal" autocomplete="off">
                <fieldset class="form">

                    <f:field bean="registerCommand" property="username"/>
                    <f:field bean="registerCommand" property="email"/>
                    <f:field bean="registerCommand" widget-type="password" property="password"/>
                    <f:field bean="registerCommand" widget-type="password" property="password2"/>
                </fieldset>
                <fieldset class="buttons">
                    <g:submitButton name="create" class="save btn btn-primary" value="${message(code: 'spring.security.ui.register.submit', default: 'Register')}" />
                </fieldset>

            </form>
        </div>
    </div>
</div>
<script>
    (function() {
        document.forms['loginForm'].elements['${usernameParameter ?: 'username'}'].focus();
    })();
</script>

</body>
</html>