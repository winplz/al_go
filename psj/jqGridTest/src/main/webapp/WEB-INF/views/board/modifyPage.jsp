<%@ page language="java" contentType="text/html; charset=UTF-8"
	pageEncoding="UTF-8"%>

<%@include file="../include/topmenu.jsp"%>
<!-- Section Background -->
<title>IGLOOSECURITY</title>
<%@ taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core"%>
<%@ taglib prefix="fn" uri="http://java.sun.com/jsp/jstl/functions"%>
<c:set var="location" value="${pageContext.request.contextPath}" />
<style>
.fileDrop {
	width: 100%;
	height: 100px;
	border: 1px dotted gray;
	background-color: #fff;
	margin: auto;
}
</style>
<section class="section-background">
	<div class="container">
		<ol class="breadcrumb">
			<li><a href="${location}/main/main">Home</a></li>
		</ol>
	</div>
	<!-- /.container -->
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
						<h3 class="box-title">글수정</h3>
					</div>
					<!-- /.box-header -->

					<form name="form1" role="form" method="post">

						<input type='hidden' name='page' value="${cri.page}"> <input
							type='hidden' name='perPageNum' value="${cri.perPageNum}">
						<input type='hidden' name='searchType' value="${cri.searchType}">
						<input type='hidden' name='keyword' value="${cri.keyword}">

						<div class="box-body">
							<div class="form-group">
								<label for="exampleInputEmail1">번호</label> <input type="text"
									name='bno' class="form-control" value="${boardDTO.bno}"
									readonly="readonly">
							</div>
							<div class="form-group">
								<label for="exampleInputEmail1">작성자</label> <input type="text"
									name="writer" class="form-control" value="${boardDTO.writer}" readonly="readonly">
							</div>
							<div class="form-group">
								<label for="exampleInputEmail1">제목</label> <input id="title" type="text"
									name='title' class="form-control" value="${boardDTO.title}">
							</div>
							<div class="form-group">
								<label for="exampleInputPassword1">내용</label>
								<textarea id="contents" class="form-control" name="content" rows="15">${boardDTO.content}</textarea>
							</div>
						</div>
						<!-- /.box-body -->
					
					<div class="box-footer">
						<button type="submit" class="btn btn-warning pull-right">취소</button>
						<button id="modifySuccess" type="submit" class="btn btn-primary pull-right" style="margin-right:5px">수정 완료</button>
					</div>
					<input type="hidden" name="${_csrf.parameterName}" value="${_csrf.token}"/>
				</form>
					<script type="text/javascript"
						src="<c:url value="/resources/assets/js/upload.js"/>"></script>
					<script src="https://cdnjs.cloudflare.com/ajax/libs/handlebars.js/3.0.1/handlebars.js"></script>

<script id="template" type="text/x-handlebars-template">
<li>
  <span class="mailbox-attachment-icon has-img"><img src="{{imgsrc}}" alt="Attachment"></span>
  <div class="mailbox-attachment-info">
	<a href="{{getLink}}" class="mailbox-attachment-name">{{fileName}}</a>
	<a href="{{fullName}}" 
     class="btn btn-default btn-xs pull-right delbtn"><i class="fa fa-fw fa-remove"></i></a>
	</span>
  </div>
</li>                
</script>    

<script>
	$(document).ready(function(){
			
		var formObj = $("form[role='form']");
		
		formObj.submit(function(event){
			event.preventDefault();
			
			var that = $(this);
			
			var str ="";
			$(".uploadedList .delbtn").each(function(index){
				 str += "<input type='hidden' name='files["+index+"]' value='"+$(this).attr("href") +"'> ";
			});
			
			that.append(str);
			console.log(str);
			
			that.get(0).submit();
		});
		

		$(".btn-warning").on("click", function(){
		  self.location = "${location}/board/list?page=${cri.page}&perPageNum=${cri.perPageNum}"+
				  "&searchType=${cri.searchType}&keyword=${cri.keyword}";
		});
	});
	var template = Handlebars.compile($("#template").html());
	$(".fileDrop").on("dragenter dragover", function(event){
		event.preventDefault();
	});
	$(".fileDrop").on("drop", function(event){
		event.preventDefault();
		
		var files = event.originalEvent.dataTransfer.files;
		
		var file = files[0];
		//console.log(file);
		
		var formData = new FormData();
		
		formData.append("file", file);	
		
		$.ajax({
			  url: '${location}/uploadAjax',
			  data: formData,
			  dataType:'text',
			  processData: false,
			  contentType: false,
			  type: 'POST',
			  success: function(data){
				  
				  var fileInfo = getFileInfo(data);
				  
				  var html = template(fileInfo);
				  
				  $(".uploadedList").append(html);
			  }
			});	
	});
	$(".uploadedList").on("click", ".delbtn", function(event){
		
		event.preventDefault();
		
		var that = $(this);
		 
		$.ajax({
		   url:"${location}/deleteFile",
		   type:"post",
		   data: {fileName:$(this).attr("href")},
		   dataType:"text",
		   success:function(result){
			   if(result == 'deleted'){
				   that.closest("li").remove();
			   }
		   }
	   });
	});
	var bno = ${boardDTO.bno};
	var template = Handlebars.compile($("#template").html());
	$.getJSON("${location}/board/getAttach/"+bno,function(list){
		$(list).each(function(){
			
			var fileInfo = getFileInfo(this);
			
			var html = template(fileInfo);
			
			 $(".uploadedList").append(html);
			
		});
	});
	$(".uploadedList").on("click", ".mailbox-attachment-info a", function(event){
		
		var fileLink = $(this).attr("href");
		
		if(checkImageType(fileLink)){
			
			event.preventDefault();
					
			var imgTag = $("#popup_img");
			imgTag.attr("src", fileLink);
			
			console.log(imgTag.attr("src"));
					
			$(".popup").show('slow');
			imgTag.addClass("show");		
		}	
	});
	$("#popup_img").on("click", function(){
		
		$(".popup").hide('slow');
		
	});	
</script>
<script>
$(document).ready(function() {
	$("#modifySuccess").click(function() {
		var title = $("#title").val();
		var contents = $("#contents").val();
		if (title == "") {
			alert("글 제목을 입력하세요.");
			$("#title").focus(); // 입력포커스 이동
			return false; // 함수 종료
		}
		if (contents == "") {
			alert("글 내용을 입력하세요.");
			$("#contents").focus();
			return false;
		}
		// 폼 내부의 데이터를 전송할 주소

		document.form1.action = "${location}/board/modifyPage"
		// 제출
		document.form1.submit();
	});
});
</script>

				</div>
				<!-- /.box -->
			</div>
			<!--/.col (left) -->

		</div>
		<!-- /.row -->
	</section>
	<!-- /.content -->
</div>
<!-- /.content-wrapper -->

<%@include file="../include/footer.jsp"%>