<%@ page language="java" contentType="text/html; charset=UTF-8"
	pageEncoding="UTF-8"%>
<!DOCTYPE html>
<html lang="kr">

  <head>  
<title>IGLOOSECURITY</title>
  </head>
 <%@ taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core"%>
<c:set var="location" value="${pageContext.request.contextPath}" />
  <body>
<%@ include file="../include/topmenu.jsp" %>
   
   <div class="container" style="margin-top:50px;">
		<h3>기본정보입력</h3>
		<br>
		<span style="color:red;">※회원가입후 등록된 사용자 정보가 아니면, 회원정보 찾기시 없는 회원으로 간주하기 때문에 반드시 정확하게 입력해주세요.</span>
		<br>
		<form  action="/user/signupSuccess" id="join_second_form" method="post" style="margin-top:30px;" >
			<div id="id_field" class="join_row" style="margin-bottom:30px;">
				<label for="id">아이디 </label>
				<input type="text" class="form-control" id="username" name="mid" placeholder="ID 입력">
				<span class="input_check"></span>
			</div>
			
			<div id="pwd_field" class="join_row" style="margin-bottom:30px;">
				<label for="pwd">비밀번호 </label>
				<input type="password" class="form-control" id="password" name="mpwd" placeholder="영문,숫자,특수문자혼합하여 8~20자리입력">
				<span class="input_check"></span>
			</div>
			
			
			<div id="name_field" class="join_row" style="margin-bottom:30px;">
				<label for="name">이름 </label>
				<input type="text" class="form-control" id="name" name="mname" placeholder="이름 입력">
				<span class="input_check"></span>
			</div>
			
			<div id="age_field" class="join_row" style="margin-bottom:30px;">
				<label for="age">나이</label>
				<input type="text" class="form-control" id="age" name="mage" placeholder="나이 입력" onkeydown="javascript:onlyNumber(this)">
				<span class="input_check"></span>
			</div>
			
			<div id="gender_field" class="join_row" style="margin-bottom:30px;">
				<label for="gender_box">성별 </label>
				<select name="mgender" class="form-control" id="gender_box">
					<option value="M">남자</option>
					<option value="W">여자</option>
				</select>
			</div>
			
			<div id="email_field" class="join_row" style="margin-bottom:30px;">
				<label for="email">이메일</label>
				<input type="text" class="form-control" name="memail" id="mEmail">
				<span class="input_check"></span>
			</div>
			
			<div id="tel_field" class="join_row" style="margin-bottom:30px;">
				<label for="tel">전화번호</label>
				<input type="text" class="form-control" id="tel" name="mtel" placeholder="'-'를 제외하고 입력하세요">
				<span class="input_check"></span>
			</div>
			
			<!-- <input type="hidden" name="authority" value="ROLE_USER"> -->
			
			<div class="submit_group">
				<input type="submit" class="btn btn-primary" id="joinBtn" value="가입">
				<a type="button" class="btn btn-danger" href="${location}/main" role="button">취소</a>
			</div>
		</form>
	</div>

<%@ include file="../include/footer.jsp" %>


  </body>

</html>
