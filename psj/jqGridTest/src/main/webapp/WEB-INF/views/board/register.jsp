<%@ page language="java" contentType="text/html; charset=UTF-8"
	pageEncoding="UTF-8"%>
<!DOCTYPE html>
<html class="noIE" lang="kr">
<head>
<title>IGLOOSECURITY</title>
<%@ taglib uri="http://www.springframework.org/security/tags" prefix="sec"%>
<%@ taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core"%>
<c:set var="location" value="${pageContext.request.contextPath}" />
<link rel="stylesheet" href="<c:url value="/resources/assets/css/mailbox.css"/>">
<style>
.fileDrop {
	width: 100%;
	height: 100px;
	border: 1px dotted gray;
	background-color: #fff;
	margin: auto;
}
</style>
</head>
<body>
	<%@ include file="../include/topmenu.jsp"%>
	<!-- Main content -->
	<section class="section-background">
	<div class="container">
		<ol class="breadcrumb">
			<li><a href="${location}/main">Home</a></li>
		</ol>
	</div>
</section>
	<div class="container" style="margin-top: 50px">
		<section class="content">
			<div class="row">
				<div class="col-md-12">
					<div class="box box-primary">
						<div class="box-header">
							<h3 class="box-title">글쓰기</h3>
						</div>

	<sec:authorize access="isAuthenticated()"> 
	  		<sec:authentication property="principal" var="user"/>
						<form id='registerForm' role="form" method="post">
							<div class="box-body">
								<div class="form-group">
									<label for="exampleInputEmail1">작성자</label> <input type="text"
										name="writer" class="form-control" value="${user.username}" readonly="readonly">
								</div>
								<div class="form-group">
									<label for="exampleInputEmail1">제목</label> <input id="title" type="text"
										name='title' class="form-control" placeholder="제목을 입력하세요">
								</div>
								<div class="form-group">
									<label for="exampleInputPassword1">내용</label>
									<textarea id="contents" class="form-control" name="content" rows="15" 
									placeholder="게시판에 글쓰기를 하는 경우, 본문에 개인정보(주민등록번호, 성명, 연락처 등)가 포함되어 게시되지 않도록 유의하시기 바랍니다.
개인정보를 포함하여 게시하는 경우에는 불특정 다수에게 개인정보가 노출되어 악용될 수 있으며,
특히, 타인의 개인정보가 노출되는 경우에는 개인정보보호법에 따라 처벌을 받을 수 있음을 알려드립니다."></textarea>
								</div>
							</div>
							<div class="box-footer">
								<a id="cancel" class="btn btn-warning pull-right" href="${location}/main">취소</a>
								<button id="registerSuccess" type="submit" class="btn btn-primary pull-right" style="margin-right:5px">완료</button>
							</div>
							<input type="hidden" name="${_csrf.parameterName}" value="${_csrf.token}"/>
						</form>
				</sec:authorize>
					</div>
				</div>
			</div>
		</section>
	</div>


	<%@ include file="../include/footer.jsp"%>
</body>
</html>
