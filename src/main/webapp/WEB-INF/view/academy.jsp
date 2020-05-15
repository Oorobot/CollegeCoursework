<%@ page language="java" contentType="text/html; charset=UTF-8"
	pageEncoding="UTF-8"%>
<%@ taglib prefix="c" uri="http://java.sun.com/jstl/core_rt"%>
<%@ taglib prefix="fn" uri="http://java.sun.com/jsp/jstl/functions"%>
<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<title>学院信息</title>
<meta http-equiv="X-UA-Compatible" content="IE=edge">
<meta name="viewport" content="width=device-width, initial-scale=1">
<link
	href="${pageContext.request.contextPath}/bootstrap/css/bootstrap.min.css"
	rel="stylesheet">
</head>
<body>
	<!-- 导航栏  -->
	<jsp:include page="admin-nav.jsp"></jsp:include>
	<!-- 表单  -->
	<div class="container" style="margin-top: 10%">
		<form class="form-horizontal"
			action="${pageContext.request.contextPath}/admin/academy"
			method="post">
			<div class="form-group">
				<label for="ano" class="col-sm-2 control-label">学院号</label>
				<div class="col-sm-10">
					<c:if test="${fn:contains(operate,'add') }">
						<input type="text" class="form-control" id="ano"
							placeholder="由两位数字组成，例如：01，02" value="${academy.ano }"
							name="academy">
					</c:if>
					<c:if test="${fn:contains(operate,'update') }">
						<input type="text" class="form-control" id="ano"
							placeholder="由两位数字组成，例如：01，02" value="${academy.ano }"
							name="academy" readonly="readonly">
					</c:if>
				</div>
			</div>
			<div class="form-group">
				<label for="name" class="col-sm-2 control-label">学院名</label>
				<div class="col-sm-10">
					<input type="text" class="form-control" id="name"
						placeholder="学院名，例如：计算机学院" value="${academy.name }" name="academy">
				</div>
			</div>
			<div class="form-group">
				<label for="address" class="col-sm-2 control-label">学院地址</label>
				<div class="col-sm-10">
					<input type="text" class="form-control" id="address"
						placeholder="地址" value="${academy.address }" name="academy">
				</div>
			</div>
			<div class="form-group">
				<label for="phone" class="col-sm-2 control-label">联系电话</label>
				<div class="col-sm-10">
					<input type="text" class="form-control" id="phone" placeholder="电话"
						value="${academy.phone }" name="academy">
				</div>
			</div>
			<div class="form-group">
				<div class="col-sm-offset-2 col-sm-10">
					<button type="submit" class="btn btn-default" name="operate"
						value="${operate }">提交</button>
				</div>
			</div>
		</form>

	</div>
</body>

<script type="text/javascript"
	src="${pageContext.request.contextPath}/jquery-3.4.1.min.js"></script>
<script
	src="${pageContext.request.contextPath}/bootstrap/js/bootstrap.min.js"></script>
</html>