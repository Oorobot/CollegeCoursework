<%@ page language="java" contentType="text/html; charset=UTF-8"
	pageEncoding="UTF-8"%>
<%@ taglib prefix="c" uri="http://java.sun.com/jstl/core_rt"%>
<%@ taglib prefix="fn" uri="http://java.sun.com/jsp/jstl/functions"%>
<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<title>学生信息</title>
<meta http-equiv="X-UA-Compatible" content="IE=edge">
<meta name="viewport" content="width=device-width, initial-scale=1">
<link
	href="${pageContext.request.contextPath}/bootstrap/css/bootstrap.min.css"
	rel="stylesheet">
</head>
<body>
	<!-- 导航栏  -->
	<c:if
		test="${fn:contains(operate,'add') or fn:contains(operate,'update') }">
		<jsp:include page="admin-nav.jsp"></jsp:include>
	</c:if>
	<c:if test="${fn:endsWith(operate,'info') }">
		<jsp:include page="student-nav.jsp"></jsp:include>
	</c:if>
	<!-- 表单  -->
	<div class="container" style="margin-top: 5%">
		<form class="form-horizontal"
			action="${pageContext.request.contextPath}/${adr }"
			method="post">
			<!-- 显示修改回馈信息 -->
			<div style="text-align: center;">
				<label style="color: red;">${error }</label>
				<label style="color: green;">${success }</label>
			</div>
			<!-- 学号  -->
			<div class="form-group">
				<label for="sno" class="col-sm-2 control-label">学号</label>
				<div class="col-sm-10">
					<c:if test="${fn:contains(operate,'add') }">
						<input type="number" class="form-control" id="sno"
							placeholder="由四位数字组成" value="${student.sno }" name="student">
					</c:if>
					<c:if test="${fn:contains(operate,'update') or fn:contains(operate,'info') }">
						<input type="number" class="form-control" id="sno"
							placeholder="由四位数字组成" value="${student.sno }" name="student"
							readonly="readonly">
					</c:if>
				</div>
			</div>
			<div class="form-group">
				<label for="name" class="col-sm-2 control-label">姓名</label>
				<div class="col-sm-10">
					<input type="text" class="form-control" id="name" placeholder="姓名"
						value="${student.name }" name="student">
				</div>
			</div>
			<div class="form-group">
				<label for="sex" class="col-sm-2 control-label">性别</label>
				<div class="col-sm-10">
					<c:if test="${empty student.sno or student.sex == false }">
						<label class="radio-inline"> <input id="sex" type="radio"
							name="student" value="false" checked>男
						</label>
						<label class="radio-inline"> <input id="sex" type="radio"
							name="student" value="true">女
						</label>
					</c:if>
					<c:if test="${student.sex == true }">
						<label class="radio-inline"> <input id="sex" type="radio"
							name="student" value="false">男
						</label>
						<label class="radio-inline"> <input id="sex" type="radio"
							name="student" value="true" checked>女
						</label>
					</c:if>
				</div>
			</div>
			<div class="form-group">
				<label for="birthday" class="col-sm-2 control-label">出生日期</label>
				<div class="col-sm-10">
					<input type="date" class="form-control" id="birthday"
						placeholder="格式：YYYY-MM-DD" value="${student.birthday }"
						name="student">
				</div>
			</div>
			<div class="form-group">
				<label for="hometown" class="col-sm-2 control-label">籍贯</label>
				<div class="col-sm-10">
					<input type="text" class="form-control" id="hometown"
						placeholder="上海、北京或广东等" value="${student.hometown }"
						name="student">
				</div>
			</div>
			<div class="form-group">
				<label for="phone" class="col-sm-2 control-label">联系电话</label>
				<div class="col-sm-10">
					<input type="text" class="form-control" id="phone"
						placeholder="电话号码" value="${student.phone }" name="student">
				</div>
			</div>

				<div class="form-group">
					<label for="hour" class="col-sm-2 control-label">学院</label>
					<div class="col-sm-10">
					<!-- 管理员可更新  -->
					<c:if
						test="${fn:contains(operate,'add') or fn:contains(operate,'update') }">
						<select class="form-control" name="student">
							<c:forEach items="${academies }" var="a">
								<c:if test="${!empty student.sno and a.ano==student.ano }">
									<option value="${a.ano }" selected="selected">${a.name }</option>
								</c:if>
								<c:if test="${empty student.sno or a.ano!=student.ano }">
									<option value="${a.ano }">${a.name }</option>
								</c:if>
							</c:forEach>
						</select>
					</c:if>
					<!-- 学生端警用可更新  -->
					<c:if test="${fn:contains(operate,'info') }">
						<select class="form-control" name="student" disabled>
							<c:forEach items="${academies }" var="a">
								<c:if test="${!empty student.sno and a.ano==student.ano }">
									<option value="${a.ano }" selected="selected">${a.name }</option>
								</c:if>
								<c:if test="${empty student.sno or a.ano!=student.ano }">
									<option value="${a.ano }">${a.name }</option>
								</c:if>
							</c:forEach>
						</select>
						<input type="hidden" name="student" value="${student.ano }">
					</c:if>
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