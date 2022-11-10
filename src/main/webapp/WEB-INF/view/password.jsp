<%@ page language="java" contentType="text/html; charset=UTF-8"
	pageEncoding="UTF-8"%>
<%@ taglib prefix="c" uri="http://java.sun.com/jstl/core_rt"%>
<%@ taglib prefix="fn" uri="http://java.sun.com/jsp/jstl/functions"%>
<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<title>修改密码</title>
<meta http-equiv="X-UA-Compatible" content="IE=edge">
<meta name="viewport" content="width=device-width, initial-scale=1">
<link
	href="${pageContext.request.contextPath}/bootstrap/css/bootstrap.min.css"
	rel="stylesheet">
</head>
<body>
	<c:if test="${fn:contains(adr,'teacher') }">
		<jsp:include page="teacher-nav.jsp"></jsp:include>
	</c:if>
	<c:if test="${fn:contains(adr,'student') }">
		<jsp:include page="student-nav.jsp"></jsp:include>
	</c:if>

	<div class="modal-dialog" role="document">
		<div class="modal-content">
			<div class="modal-header">
				<h4 class="modal-title" id="passwordModalLabel">修改密码</h4>
			</div>
			<div class="modal-body">
				<form action="${pageContext.request.contextPath}/${adr }"
					method="post">
					<div class="form-group">
						<label for="password" class="control-label">新密码</label> <input
							type="password" class="form-control" id="password"
							name="password1" maxlength="20">
					</div>
					<div class="form-group">
						<label for="password2" class="control-label">再次输入密码</label> <input
							type="password" class="form-control" id="password2"
							name="password2" maxlength="20">
					</div>
					<c:if test="${!empty error }">
						<div style="text-align: center;">
							<label style="color: red;">错误：${error }</label>
						</div>
					</c:if>
					<c:if test="${!empty success }">
						<div style="text-align: center;">
							<label style="color: green;">${success }</label>
						</div>
					</c:if>
					<div style="text-align: center;">
						<button type="submit" class="btn btn-info">确定</button>
					</div>
				</form>
			</div>
		</div>
	</div>

</body>
<script type="text/javascript"
	src="${pageContext.request.contextPath}/jquery-3.4.1.min.js"></script>
<script
	src="${pageContext.request.contextPath}/bootstrap/js/bootstrap.min.js"></script>
</html>