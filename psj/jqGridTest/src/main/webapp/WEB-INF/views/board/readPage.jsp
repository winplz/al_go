<%@ page language="java" contentType="text/html; charset=UTF-8"
	pageEncoding="UTF-8"%>

<%@ taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core"%>
<%@ taglib prefix="fmt" uri="http://java.sun.com/jsp/jstl/fmt"%>
<%@ taglib prefix="sec" uri="http://www.springframework.org/security/tags" %>
<c:set var="location" value="${pageContext.request.contextPath}" />
<title>IGLOOSECURITY</title>
<%@include file="../include/topmenu.jsp"%>
<style>
.h-100 {
    height: 0%!important;
}
</style>
<section class="section-background">
	<div class="container">
		<ol class="breadcrumb">
			<li><a href="${location}/main">Home</a></li>
		</ol>
	</div>
</section>

<div class="container" style="margin-top: 50px">
	<!-- Main content -->
	<section class="content">
		<div class="row">
			<!-- left column -->
			<div class="col-md-12">
				<!-- general form elements -->
				<div class="box box-primary">
					<div class="box-header">
						<h3 class="box-title">글보기</h3>
					</div>
					<!-- /.box-header -->

					<form role="form" action="modifyPage" method="post">
						<input type='hidden' name='bno' value="${boardDTO.bno}">
					</form>

					<div class="box-body">
						<div class="form-group">
							<label for="exampleInputEmail1">번호</label> <input type="text"
								name='bno' class="form-control" value="${boardDTO.bno}"
								readonly="readonly">
						</div>
						<div class="form-group">
							<label for="exampleInputEmail1">작성자</label> <input type="text"
								name="writer" class="form-control" value="${boardDTO.writer}"
								readonly="readonly">
						</div>
						<div class="form-group">
							<label for="exampleInputEmail1">제목</label> <input type="text"
								name='title' class="form-control" value="${boardDTO.title}"
								readonly="readonly">
						</div>
						<div class="form-group">
							<label for="exampleInputPassword1">내용</label>
							<textarea class="form-control" name="content" rows="15"
								readonly="readonly">${boardDTO.content}</textarea>
						</div>
					</div>
			
			
			<ul id="imgBox" class="mailbox-attachments clearfix uploadedList" style="list-style:none;"></ul>
					<!-- /.box-body -->
					<div class="box-footer" style="margin-top: 50px">
					<sec:authorize access="isAuthenticated()"> 
					<sec:authentication property="principal" var="user"/> 
					<c:if test="${user.username == boardDTO.writer  || sessionScope.SPRING_SECURITY_CONTEXT.authentication.authorities=='[ROLE_ADMIN]'}">
						<button type="submit" class="btn btn-warning" id="postModBtn">수정</button>
						<button type="submit" class="btn btn-danger" id="postDelBtn">삭제</button>
					</c:if>
					</sec:authorize>
						<a href="${location}/main" type="submit" class="btn btn-primary" id="postListBtn">목록보기</a>
					</div>
					<hr>
						<div class='text-center'>
							<ul id="pagination" class="pagination pagination-sm no-margin ">
							</ul>
						</div>
					</div>
				</div>
			</div>
		</div>
	</section>
</div>
	<form role="form" action="modifyPage" method="post">
		<input type='hidden' name='bno' value="${boardDTO.bno}"> 
	</form>

<%@include file="../include/footer.jsp"%>

<script>
$(document).ready(function(){
	
	var formObj = $("form[role='form']");
	
	console.log(formObj);
	
	$("#postModBtn").on("click", function(){
		formObj.attr("action", "${location}/board/modifyPage");
		formObj.attr("method", "get");		
		formObj.submit();
	});
	$("#postDelBtn").on("click", function(){
		if (confirm("해당 글을 삭제하시겠습니까?") == true){    //확인
			formObj.attr("action", "${location}/board/removePage");
			formObj.submit();
		}else{   //취소
		    return;
		}
	});
});
</script>