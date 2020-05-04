<%@ page language="java" contentType="text/html; charset=UTF-8"
	pageEncoding="UTF-8"%>
<%@ taglib prefix="c" uri="http://java.sun.com/jstl/core_rt"%>
<%@ taglib prefix="fn" uri="http://java.sun.com/jsp/jstl/functions"%>
<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<title>学院操作</title>
<meta http-equiv="X-UA-Compatible" content="IE=edge">
<meta name="viewport" content="width=device-width, initial-scale=1">
<link
	href="${pageContext.request.contextPath}/bootstrap/css/bootstrap.min.css"
	rel="stylesheet">
</head>
<body>
	<div class="container" style="margin-top: 10%">
		<form class="form-horizontal"
			action="${pageContext.request.contextPath}/admin/teacher"
			method="post">
			<div class="form-group">
				<label for="sno" class="col-sm-2 control-label">工号</label>
				<div class="col-sm-10">
					<c:if test="${fn:contains(operate,'add') }">
						<input type="number" class="form-control" id="tno"
							placeholder="由四位数字组成" value="${teacher.tno }" name="teacher">
					</c:if>
					<c:if test="${fn:contains(operate,'update') }">
						<input type="number" class="form-control" id="tno"
							placeholder="由四位数字组成" value="${teacher.tno }" name="teacher"
							readonly>
					</c:if>
				</div>
			</div>
			<div class="form-group">
				<label for="name" class="col-sm-2 control-label">姓名</label>
				<div class="col-sm-10">
					<input type="text" class="form-control" id="name" placeholder="姓名"
						value="${teacher.name }" name="teacher">
				</div>
			</div>
			<div class="form-group">
				<label for="sex" class="col-sm-2 control-label">性别</label>
				<div class="col-sm-10">
					<c:if test="${empty teacher.tno or teacher.sex == false }">
						<label class="radio-inline"> <input id="sex" type="radio"
							name="teacher" value="false" checked>男
						</label>
						<label class="radio-inline"> <input id="sex" type="radio"
							name="teacher" value="true">女
						</label>
					</c:if>
					<c:if test="${teacher.sex == true }">
						<label class="radio-inline"> <input id="sex" type="radio"
							name="teacher" value="false">男
						</label>
						<label class="radio-inline"> <input id="sex" type="radio"
							name="teacher" value="true" checked>女
						</label>
					</c:if>
				</div>
			</div>
			<div class="form-group">
				<label for="birthday" class="col-sm-2 control-label">出生日期</label>
				<div class="col-sm-10">
					<input type="date" class="form-control" id="birthday"
						placeholder="格式：YYYY-MM-DD" value="${teacher.birthday }" name="teacher">
				</div>
			</div>
			<div class="form-group">
				<label for="position" class="col-sm-2 control-label">职称</label>
				<div class="col-sm-10">
					<input type="text" class="form-control" id="position"
						placeholder="例如：教师，副教授 ..." value="${teacher.position }" name="teacher">
				</div>
			</div>
			<div class="form-group">
				<label for="salary" class="col-sm-2 control-label">工资</label>
				<div class="col-sm-10">
					<input type="text" class="form-control" id="salary"
						placeholder="例如：2000.00" value="${student.phone }" name="teacher">
				</div>
			</div>
			
			
			<div class="form-group">
				<label for="hour" class="col-sm-2 control-label">学院</label>
				<div class="col-sm-10">
					<select class="form-control" name="teacher">
						<c:forEach items="${academies }" var="a">
							<c:if test="${!empty teacher.tno }">
								<c:if test="${a.ano==teacher.ano }">
									<option value="${a.ano }" selected="selected">${a.name }</option>
								</c:if>
								<c:if test="${a.ano!=teacher.ano }">
									<option value="${a.ano }">${a.name }</option>
								</c:if>
							</c:if>
							<c:if test="${empty teacher.tno }">
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