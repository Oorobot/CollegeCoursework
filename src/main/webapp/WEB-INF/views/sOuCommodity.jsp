<%@ page language="java" contentType="text/html; charset=UTF-8"
	pageEncoding="UTF-8"%>
<%@ taglib prefix="c" uri="http://java.sun.com/jstl/core_rt"%>
<%@ taglib prefix="form" uri="http://www.springframework.org/tags/form"%>
<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<title>Insert title here</title>
</head>
<body>
	<form:form
		action="${pageContext.request.contextPath}/successC"
		method="POST" enctype="multipart/form-data" modelAttribute="commodity">
		<form:hidden path="commodityId" />
		<form:hidden path="userId" />
		<input type="hidden" name="oldImage" value="${oldImage }">
		商品名：<form:input path="commodityName" />
		<br>
		<br>
		商品价格：<form:input path="commodityPrice" />
		<br>
		<br>
		商品类别：<form:select path="commodityClassId" items="${ccMap }" />
		<br>
		<br>
		商品图片：<input type="file" name="file" />
		<c:if test="${commodity.commodityId != null }">
			<input type="image"
				src="${pageContext.request.contextPath}/${commodity.commodityImage}"
				width="200px" height="250px">
		</c:if>
		<br>
		<br>
		商品描述：<form:input path="commodityDepict" />
		<br>
		<br>
		<input type="submit" name="确定">
	</form:form>
</body>
</html>