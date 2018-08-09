<%@ page language="java" contentType="text/html; charset=UTF-8"
	pageEncoding="UTF-8"%>
<!DOCTYPE html>
<html lang="kr">
<head>
<title>IGLOOSECURITY</title>
<%@ taglib uri="http://www.springframework.org/security/tags"
	prefix="sec"%>
<%@ taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core"%>
<c:set var="location" value="${pageContext.request.contextPath}" />
<!-- jqGrid -->
<link rel="stylesheet" type="text/css" media="screen"
	href="/resources/assets/css/ui.jqgrid.css"></link>
<script type="text/javascript"
	src="<c:url value="/resources/assets/js/jquery-1.11.0.min.js"/>"></script>
<script type="text/javascript"
	src="<c:url value="/resources/assets/js/jquery.jqGrid.min.js"/>"></script>
<script type="text/javascript"
	src="<c:url value="/resources/assets/js/grid.locale-kr.js"/>"></script>

<script type="text/javascript">
	jQuery(function($) {
		$("#list").jqGrid({
			url : "${location}/contents/getList",
			datatype : "json",
			mtype : "GET",
			colNames : [ "번호", "제목", "작성자", "등록일", "조회수" ],
			colModel : [ {
				name : "bno",
				index : "bno",
				width : 200,
				align : "center",
				sortable:true,
				editable:false,
				formatter:myLink
			}, {
				name : "title",
				index : "title",
				width : 650,
				align : "center"
			}, {
				name : "writer",
				index : "writer",
				width : 280,
				align : "center"
			}, {
				name : "regdate",
				index : "regdate",
				width : 280,
				align : "center",
				formatter:"date",
				formatoptions: { srcformat: "UTF-8", newformat: "Y-m-d h:m A" }

			}, {
				name : "viewcnt",
				index : "viewcnt",
				width : 100,
				align : "center",
				formatter : fontColorFormat
			} ],
			pager : "#pager",
			rowNum : 100,
			rowList : [ 100, 200, 300, 400, 500, 600, 700, 800, 900, 1000 ],
			sortname : "bno",
			sortorder : "desc",
			viewrecords : true,
			gridview : true,
			autoencode : true,
			caption : "이글루 시큐리티 게시판"
		});
	});

	/* 게시판 번호 글씨색 */
	function fontColorFormat(cellvalue, options, rowObject) {
		var color = "red";
		var cellHtml = "<span style='color:" + color + "' originalValue='" + cellvalue + "'>"
				+ cellvalue + "</span>";
		return cellHtml;
	}
	/* 링크 함수 호출*/
	function myLink(cellvalue,options,rowObject){
	
		
		url='<a href="${location}/board/readPage?bno='+cellvalue+'">'+cellvalue+'</a>';
		return url;
	}
</script>
<style>
.ui-jqgrid-bdiv{
    height: 350px !important;
    width: 1510px;
}
</style>
</head>
<body>
	<%@ include file="include/topmenu.jsp"%>


	<table id="list">
		<tr>
			<td></td>
		</tr>
	</table>

	<div id="pager"></div>


<sec:authorize access="isAuthenticated()">
	<a href="${location}/board/register" class="btn btn-primary pull-right"
		style="margin-right: 20px;">글쓰기</a>
</sec:authorize>

	<%@ include file="include/footer.jsp"%>
</body>
<script type="text/javascript">
	var result = '${msg}';

	if (result == 'usersignup success') {
		alert("회원가입을 축하합니다! 로그인해주세요!");
	} else if (result == 'login success') {
		alert("${sessionScope.mid}님 환영합니다. ");
	}
</script>

</html>
