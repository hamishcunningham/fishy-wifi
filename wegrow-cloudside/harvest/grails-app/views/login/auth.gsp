<!DOCTYPE html>

<html>
<head>
    <meta name="layout" content="main"/>
    <title><g:message code="springSecurity.login.title"/></title>
</head>
<body>
<div id="login">
    <div class="container">
        <div class="col-md-8 col-md-offset-2">
        <h1><g:message code='springSecurity.login.header'/></h1>

        <g:if test='${flash.message}'>
            <div class="login_message alert">${flash.message}</div>
        </g:if>

        <form action="${postUrl ?: '/login/authenticate'}" method="POST" id="loginForm" class="form-horizontal" autocomplete="off">
            <div class="form-group">
                <label for="username" class="col-sm-2 control-label"><g:message code='springSecurity.login.username.label'/></label>
                <div class="col-sm-10">
                    <input type="text" class="text_ form-control" name="${usernameParameter ?: 'username'}" id="username"/>
                </div>
            </div>

            <div class="form-group">
                <label for="password" class="col-sm-2 control-label"><g:message code='springSecurity.login.password.label'/></label>
                <div class="col-sm-10">
                    <input type="password" class="text_ form-control" name="${passwordParameter ?: 'password'}" id="password"/>
                </div>
            </div>

            <div class="form-group" id="remember_me_holder">

                <div class="col-sm-offset-2 col-sm-10">

                    <label for="remember_me">
                        <input type="checkbox" class="chk" name="${rememberMeParameter ?: 'remember-me'}" id="remember_me" <g:if test='${hasCookie}'>checked="checked"</g:if>/>

                        <g:message code='springSecurity.login.remember.me.label'/></label>
                </div>

            </div>

            <div class="form-group">
                <label class="col-sm-2" class="col-sm-2"> </label>

                <div class="col-sm-10">
                    <input type="submit" id="submit" class="btn btn-primary" value="${message(code: 'springSecurity.login.button')}"/>
                </div>
            </div>
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