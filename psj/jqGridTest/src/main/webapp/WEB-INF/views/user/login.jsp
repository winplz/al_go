<%@ page language="java" contentType="text/html; charset=UTF-8"
	pageEncoding="UTF-8"%>
<!DOCTYPE html>
<html class="noIE" lang="kr">
<head>
<title>IGLOOSECURITY</title>

<%@ taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core"%>
<%@ taglib prefix="fmt" uri="http://java.sun.com/jsp/jstl/fmt"%>
<c:set var="location" value="${pageContext.request.contextPath}" />
<style>
.panel-primary {
	box-shadow: 0 4px 58px 0 rgba(3, 2, 1, 0.2), 0 6px 20px 0 rgba(1, 1, 1, 0.39) !important;
}
</style>
</head>
<body>
	<%@ include file="../include/topmenu.jsp"%>

	<div class="container" style="margin-top: 50px">

			<div class="panel panel-primary">
				<div class="panel-heading">
					<p>환영합니다!</p>
				</div>
				<br>
				<div class="panel-body" style="margin-top:30px; margin-bottom:30px;">

					<form action="user/loginSuccess" id="login-form" role="form" name="form1" method="post">
						<div class="input-group">
							<span class="input-group-addon"><i class="fa fa-user fa"
								aria-hidden="true"></i></span> <input type="text" class="form-control"
								name="mid" id="userId" placeholder="아이디를 입력하세요." />
						</div>
						<br>
						<div class="input-group">
							<span class="input-group-addon"><i
								class="fa fa-lock fa-lg" aria-hidden="true"></i></span> <input
								type="password" class="form-control" name="mpwd" id="userPw"
								placeholder="비밀번호를 입력하세요." />
						</div>


						<br>
						<div class="checkbox icheck">
							<label><input type="checkbox" id="remember_me"
								name="_spring_security_remember_me"> 사용자 정보 저장 </label>
						</div>
						<br>
						<div>
							<button id="btnLogin" type="submit"
								class="form-control btn btn-primary">로그인</button>
						</div>
						<br>
					</form>
				</div>
		</div>
</div>
<%@ include file="../include/footer.jsp"%>
<script type="text/javascript">
	var result = '${msg}';

	if (result == 'failure') {
		alert("사용자 정보가 일치하지않습니다.");
	}
	</script>
</body>
</html>