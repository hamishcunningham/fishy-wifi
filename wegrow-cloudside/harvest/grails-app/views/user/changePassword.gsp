<!DOCTYPE html>
<html>
    <head>
        <meta name="layout" content="main" />
        <g:set var="entityName" value="${message(code: 'growingSpace.label', default: 'GrowingSpace')}" />
        <title><g:message code="default.create.label" args="[entityName]" /></title>
    </head>
    <body>
		      <g:if test='${flash.message}'>
		      <div class='login_message'>${flash.message}</div>
		      </g:if>
		      <div class='fheader'>Change your password</div>
		      <g:form action='updatePassword' id='passwordResetForm' class='cssform' autocomplete='off'>
		         <p>
		            <label for='username'>Username</label>
		            <span class='text_'>${username}</span>
		         </p>
		         <p>
		            <label for='password'>Current Password</label>
		            <g:passwordField name='password' class='text_' />
		         </p>
		         <p>
		            <label for='password'>New Password</label>
		            <g:passwordField name='password_new' class='text_' />
		         </p>
		         <p>
		            <label for='password'>New Password (again)</label>
		            <g:passwordField name='password_new_2' class='text_' />
		         </p>
		         <p>
		            <input type='submit' value='Submit' class="btn btn-primary"/>
		         </p>
		      </g:form>
    </body>
</html>