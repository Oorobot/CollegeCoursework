<%@ page language="java" contentType="text/html; charset=UTF-8"
	pageEncoding="UTF-8"%>
<%@ taglib prefix="form" uri="http://www.springframework.org/tags/form"%>
<%@ taglib prefix="c" uri="http://java.sun.com/jstl/core_rt"%>
<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<title>Insert title here</title>
</head>
<body>

	<form:form
		action="${pageContext.request.contextPath}/admin/commodityClasses"
		method="POST" modelAttribute="commodityClass">
		<form:hidden path="commodityClassId" />
		商品类名：<form:input path="commodityClassName" />
		<input type="submit" value="确定">
	</form:form>

</body>
</html>