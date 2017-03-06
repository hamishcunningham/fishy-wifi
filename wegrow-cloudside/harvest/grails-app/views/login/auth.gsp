<!DOCTYPE html>

<html>
<head>
    <meta name="layout" content="12col"/>
    <title><g:message code="springSecurity.login.title"/></title>
</head>

<body>
<div id="login">
    <div class="row text-center">
        <h1><g:message code='springSecurity.login.header'/></h1>
    </div>
    <div class="col-md-4 col-md-offset-4">

        <g:if test='${flash.message}'>
            <div class="login_message alert">${flash.message}</div>
        </g:if>

        <form action="${postUrl ?: '/login/authenticate'}" method="POST" id="loginForm" class="" autocomplete="off">
            <div class="form-group">
                <label for="username" class="control-label"><g:message
                        code='springSecurity.login.username.label'/></label>
                <input type="text" class="text_ form-control" name="${usernameParameter ?: 'username'}" id="username"/>
            </div>

            <div class="form-group">
                <label for="password" class="control-label"><g:message
                        code='springSecurity.login.password.label'/></label>
                <input type="password" class="text_ form-control" name="${passwordParameter ?: 'password'}"
                       id="password"/>
            </div>

            <div class="form-group" id="remember_me_holder">
                <label for="remember_me">
                    <input type="checkbox" class="chk" name="${rememberMeParameter ?: 'remember-me'}" id="remember_me"
                           <g:if test='${hasCookie}'>checked="checked"</g:if>/>

                    <g:message code='springSecurity.login.remember.me.label'/></label>
            </div>

            <div class="form-group text-center">
                <input type="submit" id="submit" class="btn btn-primary btn-lg"
                       value="${message(code: 'springSecurity.login.button')}"/>
            </div>

            <div class="form-group text-center">
                <g:link class="" uri="/register/forgotPassword">
                    ${message(code: 'spring.security.ui.login.forgotPassword')}
                </g:link>
                <br/>
                <g:link class="" uri="/register/register">
                    ${message(code: 'spring.security.ui.login.register')}
                </g:link>

            </div>
        </form>
    </div>
</div>
<script>
    (function () {
        document.forms['loginForm'].elements['${usernameParameter ?: 'username'}'].focus();
    })();
</script>
</body>
</html>