<%@ page language="java" contentType="text/html; charset=UTF-8"
	pageEncoding="UTF-8"%>
<%@ taglib prefix="c" uri="http://java.sun.com/jstl/core_rt"%>
<%@ taglib prefix="fn" uri="http://java.sun.com/jsp/jstl/functions"%>
<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<title>课程信息</title>
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
			action="${pageContext.request.contextPath}/admin/course"
			method="post">
			<div class="form-group">
				<label for="cno" class="col-sm-2 control-label">课程号</label>
				<div class="col-sm-10">
					<c:if test="${fn:contains(operate,'add') }">
						<input type="number" class="form-control" id="cno"
							placeholder="由八位数字组成" value="${course.cno }" name="course">
					</c:if>
					<c:if test="${fn:contains(operate,'update') }">
						<input type="number" class="form-control" id="cno"
							placeholder="由八位数字组成" value="${course.cno }" name="course"
							readonly="readonly">
					</c:if>
				</div>
			</div>
			<div class="form-group">
				<label for="name" class="col-sm-2 control-label">课程名</label>
				<div class="col-sm-10">
					<input type="text" class="form-control" id="name" placeholder="课程名"
						value="${course.name }" name="course">
				</div>
			</div>
			<div class="form-group">
				<label for="credit" class="col-sm-2 control-label">学分</label>
				<div class="col-sm-10">
					<input type="number" class="form-control" id="credit"
						placeholder="学分" value="${course.credit }" name="course">
				</div>
			</div>
			<div class="form-group">
				<label for="hour" class="col-sm-2 control-label">学时</label>
				<div class="col-sm-10">
					<input type="number" class="form-control" id="hour"
						placeholder="学时" value="${course.hour }" name="course">
				</div>
			</div>
			<div class="form-group">
				<label for="hour" class="col-sm-2 control-label">学院</label>
				<div class="col-sm-10">
					<select class="form-control" name="course">
						<c:forEach items="${academies }" var="a">
							<c:if test="${!empty course.cno and a.ano==course.ano }">
								<option value="${a.ano }" selected="selected">${a.name }</option>
							</c:if>
							<c:if test="${empty course.cno or a.ano!=course.ano }">
								<option value="${a.ano }">${a.name }</option>
							</c:if>
						</c:forEach>
					</select>
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