<html>
<head>
    <meta name="layout" content="12col"/>
    <title><g:message code="spring.security.ui.register.title"/></title>
</head>

<body>

<div id="login">
    <div class="row">
        <g:if test='${emailSent}'>
            <div class="col-md-6 col-md-offset-3 text-center">
                <g:message code='spring.security.ui.forgotPassword.sent'/>
            </div>
        </g:if>
        <g:else>
            <h1 class="text-center"><g:message code='spring.security.ui.forgotPassword.title'/></h1>

            <div class="col-md-6 col-md-offset-3 text-center">
                <g:message code="spring.security.ui.forgotPassword.description"/>
            </div>
        </g:else>

    </div>

<div class="col-md-4 col-md-offset-4">

    <g:if test='${!emailSent}'>

        <form action="/register/forgotPassword" method="post" name="forgotPasswordForm" id="forgotPasswordForm"
              autocomplete="off">
            <div class="form-group">
                <label for="username"><g:message code="spring.security.ui.forgotPassword.username"/></label>
                <input type="text" class="form-control" name="username" value="" size="25" id="username"/>
            </div>

            <div class="form-group text-center">
                <input type='submit' class='btn btn-primary btn-lg'
                       value='${message(code: "spring.security.ui.forgotPassword.submit")}'
                       id='submit_submit' class='s2ui_hidden_button'>

            </div>

        </form>

        </div>
    </g:if>
</div>

</body>
</html>
