<%@ page language="java" contentType="text/html; charset=UTF-8"
	pageEncoding="UTF-8"%>
<%@ taglib prefix="form" uri="http://www.springframework.org/tags/form"%>
<%@ taglib prefix="c" uri="http://java.sun.com/jstl/core_rt"%>
<%@page import="java.util.Map"%>
<%@page import="java.util.HashMap"%>
<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<title>Insert title here</title>
</head>
<body>

	<h1>个人信息</h1>
	<br>
	<p>账号 ： ${user.username }</p>
	<form:form action="${pageContext.request.contextPath}/successU" method="POST"
		modelAttribute="user">
		<form:hidden path="userId" />
		<form:hidden path="username" />
                  密码:<form:input path="password" />
		<br>
		<br>
                  用户名:<form:input path="name" />
		<br>
		<br>
		<%
			Map<Boolean, String> sexs = new HashMap<Boolean, String>();
				sexs.put(true, "男");
				sexs.put(false, "女");
				request.setAttribute("sexs", sexs);
		%>		
                  性别:<form:radiobuttons path="sex" items="${sexs }" />
		<br>
		<br>
                  电话号码：<form:input path="phone" />
		<br>
		<br>
                  地址：<form:textarea path="address" />
		<br>
		<br>
		<input type="submit" value="确定">
	</form:form>
	<br>
	<form action="${pageContext.request.contextPath}/home?curPage=1">
		<input type="submit" value="取消">
	</form>
</body>
</html>