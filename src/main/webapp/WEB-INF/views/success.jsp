<%@ page language="java" contentType="text/html; charset=UTF-8"
	pageEncoding="UTF-8"%>
<%@ taglib prefix="c" uri="http://java.sun.com/jstl/core_rt"%>
<%@ taglib prefix="fn" uri="http://java.sun.com/jsp/jstl/functions"%>
<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<title>Insert title here</title>
</head>
<body>
	<h1>${msg }</h1>
	<c:if test="${!empty adr1 }"></c:if>
	<a href="${pageContext.request.contextPath}/${adr1 }">${msg2 }</a>
	<a href="${pageContext.request.contextPath}/${adr }">${msg1 }</a>
</body>
</html>