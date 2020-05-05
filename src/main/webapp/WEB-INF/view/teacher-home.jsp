<%@page import="com.fasterxml.jackson.annotation.JsonInclude.Include"%>
<%@ page language="java" contentType="text/html; charset=UTF-8"
	pageEncoding="UTF-8"%>
<%@taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core"%>
<%@ taglib prefix="fn" uri="http://java.sun.com/jsp/jstl/functions"%>
<!DOCTYPE html>
<html>
<head>
<meta charset="utf-8">
<title>管理员</title>
<meta http-equiv="X-UA-Compatible" content="IE=edge">
<meta name="viewport" content="width=device-width, initial-scale=1">
<link
	href="${pageContext.request.contextPath}/bootstrap/css/bootstrap.min.css"
	rel="stylesheet">
</head>
<body>
	<!-- 导航栏  -->
	<jsp:include page="teacher-nav.jsp"></jsp:include>
	<c:if test="${empty category }">
		<div class="container">
			<h1>
				欢迎登录！！！<small>教师管理页面</small>
			</h1>
			<!-- modal -->
			<!-- Button trigger modal -->
			<button type="button" class="btn btn-info btn-lg" data-toggle="modal"
				data-target="#infoModal">查看个人信息</button>
			<label style="color: green;">${success }</label>
		</div>

	</c:if>
	<!-- Modal -->
	<div class="modal fade" id="infoModal" tabindex="-1" role="dialog"
		aria-labelledby="infoModalLabel">
		<div class="modal-dialog" role="document">
			<div class="modal-content">
				<div class="modal-header">
					<button type="button" class="close" data-dismiss="modal"
						aria-label="Close">
						<span aria-hidden="true">&times;</span>
					</button>
					<h4 class="modal-title" id="infoModalLabel">个人信息</h4>
				</div>
				<div class="modal-body">
					<dl class="dl-horizontal">
						<dt>工号</dt>
						<dd>${teacher.tno }</dd>
						<dt>姓名</dt>
						<dd>${teacher.name }</dd>
						<dt>性别</dt>
						<dd>${teacher.sex == false ? "男":"女" }</dd>
						<dt>出生日期</dt>
						<dd>${teacher.birthday }</dd>
						<dt>职称</dt>
						<dd>${teacher.position }</dd>
						<dt>工资</dt>
						<dd>${teacher.salary }</dd>
						<dt>学院</dt>
						<dd>${academyName }</dd>
					</dl>
				</div>
				<div class="modal-footer" style="text-align: center;">
					<button type="button" class="btn btn-default" data-dismiss="modal">Close</button>
				</div>
			</div>
		</div>
	</div>
</body>
<c:if test="${empty category}">
	<script type="text/javascript"
		src="${pageContext.request.contextPath}/jquery-3.4.1.min.js"></script>
	<script
		src="${pageContext.request.contextPath}/bootstrap/js/bootstrap.min.js"></script>
</c:if>
</html>