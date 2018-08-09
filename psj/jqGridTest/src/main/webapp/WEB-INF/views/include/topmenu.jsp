<%@ page language="java" contentType="text/html; charset=UTF-8"
	pageEncoding="UTF-8"%>
<meta charset="utf-8">
<meta name="viewport"
	content="width=device-width, initial-scale=1, shrink-to-fit=no">
<meta name="description" content="">
<meta name="author" content="">
<%@ taglib uri="http://www.springframework.org/security/tags" prefix="sec"%>
<%@ taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core"%>
<link
	href="<c:url value="/resources/assets/vendor/bootstrap/css/bootstrap.min.css"/>"
	rel="stylesheet" />
<link
	href="<c:url value="/resources/assets/vendor/font-awesome/css/font-awesome.min.css"/>"
	rel="stylesheet" type="text/css">
<link
	href="<c:url value="/resources/assets/vendor/simple-line-icons/css/simple-line-icons.css"/>"
	rel="stylesheet" type="text/css">
<link
	href="https://fonts.googleapis.com/css?family=Lato:300,400,700,300italic,400italic,700italic"
	rel="stylesheet" type="text/css">
<link href="<c:url value="/resources/assets/css/landing-page.min.css"/>"
	rel="stylesheet">
<link rel="stylesheet" type="text/css"
	href="https://maxcdn.bootstrapcdn.com/font-awesome/4.7.0/css/font-awesome.min.css">
<c:set var="location" value="${pageContext.request.contextPath}" />
<nav class="navbar navbar-light bg-light static-top">
	<div class="container">
		<a class="navbar-brand" href="${location}/main">IGLOOSECURITY</a> 
<div class="btn-group">
		<sec:authorize access="isAnonymous()">
		<a class="btn btn-primary btn-sm" href="${location}/user/signup" style="margin-right:10px;"><i
									class="fa fa-user-plus fa-lg" aria-hidden="true"></i>&nbsp;회원가입</a>
		</sec:authorize>
		<sec:authorize access="isAnonymous()">
		<a class="btn btn-primary btn-sm" href="${location}/user/login"> <i
									class="fa fa-user-o fa-lg" aria-hidden="true"></i>&nbsp;로그인</a>
		</sec:authorize>
		<sec:authorize access="isAuthenticated()">
		<a class="btn btn-warning btn-sm" href="javascript:logOutCheck()"> <i
									class="fa fa-sign-out fa-lg" aria-hidden="true"></i> &nbsp;로그아웃</a>
		</sec:authorize>
</div>

	</div>
</nav>
<script>
function logOutCheck(){
	if(confirm("${sessionScope.SPRING_SECURITY_CONTEXT.authentication.principal.username}님 정말 로그아웃 하시겠습니까?")){
		location.href="${location}/user/logout";
	}else{
		return;
	}
}
</script>
<!-- Masthead -->
<header class="masthead text-white text-center">
	<div class="overlay"></div>
	<div class="container"></div>
</header>