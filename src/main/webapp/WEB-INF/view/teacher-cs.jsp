<%@ page language="java" contentType="text/html; charset=UTF-8"
	pageEncoding="UTF-8"%>
<%@ taglib prefix="c" uri="http://java.sun.com/jstl/core_rt"%>
<%@ taglib prefix="fn" uri="http://java.sun.com/jsp/jstl/functions"%>
<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<title>开课</title>
<meta http-equiv="X-UA-Compatible" content="IE=edge">
<meta name="viewport" content="width=device-width, initial-scale=1">
<link
	href="${pageContext.request.contextPath}/bootstrap/css/bootstrap.min.css"
	rel="stylesheet">
</head>
<body>
	<jsp:include page="teacher-nav.jsp"></jsp:include>
	<!-- 表单  -->
	<div class="container" style="margin-top: 5%">
		<form class="form-horizontal"
			action="${pageContext.request.contextPath}/teacher/${adr }"
			method="post">
			<c:if test="${!empty error }">
				<div style="text-align: center;">
					<label style="color: red;">错误：${error }</label>
				</div>
			</c:if>
			<div class="form-group">
				<label for="hour" class="col-sm-2 control-label">学期</label>
				<div class="col-sm-10">
					<c:if test="${fn:endsWith(operate,'add') }">
						<select class="form-control" name="cs">
							<c:forEach items="${terms }" var="t">
								<option value="${t.id }">${t.term }</option>
							</c:forEach>
						</select>
					</c:if>
					<c:if test="${fn:endsWith(operate,'update') }">
						<input class="form-control" type="text" value="${term.term }"
							readonly>
						<input type="hidden" name="cs" value="${term.id }">
					</c:if>
				</div>
			</div>
			<div class="form-group">
				<label for="cno" class="col-sm-2 control-label">课程号</label>
				<div class="col-sm-10">
					<input type="number" class="form-control" id="cno"
						placeholder="由八位数字组成" value="${course.cno }" name="cno" readonly>
				</div>
			</div>
			<div class="form-group">
				<label for="name" class="col-sm-2 control-label">课程名</label>
				<div class="col-sm-10">
					<input type="text" class="form-control" id="name" placeholder="课程名"
						value="${course.name }" readonly>
				</div>
			</div>
			<div class="form-group">
				<label for="credit" class="col-sm-2 control-label">学分</label>
				<div class="col-sm-10">
					<input type="number" class="form-control" id="credit"
						placeholder="学分" value="${course.credit }" readonly>
				</div>
			</div>
			<div class="form-group">
				<label for="hour" class="col-sm-2 control-label">学时</label>
				<div class="col-sm-10">
					<input type="number" class="form-control" id="hour"
						placeholder="学时" value="${course.hour }" readonly>
				</div>
			</div>
			<div class="form-group">
				<label for="time" class="col-sm-2 control-label">上课时间</label>
				<div class="col-sm-10">
					<input type="text" class="form-control" id="time"
						placeholder="上课时间" name="cs" value="${CourseSchedule.time }">
				</div>
			</div>
			<div class="form-group">
				<label for="classroom" class="col-sm-2 control-label">上课地点</label>
				<div class="col-sm-10">
					<input type="text" class="form-control" id="classroom"
						placeholder="上课地点" name="cs" value="${CourseSchedule.classroom }">
				</div>
			</div>
			<div class="form-group">
				<label for="num" class="col-sm-2 control-label">限制人数</label>
				<div class="col-sm-10">
					<input type="number" class="form-control" id="num"
						placeholder="上课人数" name="cs" value="${CourseSchedule.num }">
				</div>
			</div>
			<div class="form-group">
				<label for="weight" class="col-sm-2 control-label">平时成绩权重</label>
				<div class="col-sm-10">
					<input type="number" class="form-control" id="weight"
						placeholder="请填写0~100之间的整数" name="cs" value="${CourseSchedule.weight }">
				</div>
			</div>
			<div class="form-group">
				<div class="col-sm-offset-2 col-sm-10">
					<button type="submit" class="btn btn-default">提交</button>
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