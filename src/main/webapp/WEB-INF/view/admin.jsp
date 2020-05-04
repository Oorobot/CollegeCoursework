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
	<jsp:include page="admin-nav.jsp"></jsp:include>
	<!-- 主体内容  -->
	<c:if test="${fn:contains(category,'term') }">
		<jsp:include page="term.jsp"></jsp:include>
	</c:if>
	<c:if test="${fn:contains(category,'academy') }">
		<jsp:include page="academies.jsp"></jsp:include>
	</c:if>
	<c:if test="${fn:contains(category,'course') }">
		<jsp:include page="courses.jsp"></jsp:include>
	</c:if>
	<c:if test="${fn:contains(category,'student') }">
		<jsp:include page="students.jsp"></jsp:include>
	</c:if>
	<c:if test="${fn:contains(category,'teacher') }">
		<jsp:include page="teachers.jsp"></jsp:include>
	</c:if>
</body>
<c:if test="${empty category}">
<script type="text/javascript"
	src="${pageContext.request.contextPath}/jquery-3.4.1.min.js"></script>
<script
	src="${pageContext.request.contextPath}/bootstrap/js/bootstrap.min.js"></script>
</c:if>
</html>