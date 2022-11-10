<%@ page language="java" contentType="text/html; charset=UTF-8"
	pageEncoding="UTF-8"%>
<%@ taglib prefix="form" uri="http://www.springframework.org/tags/form"%>
<%@ taglib prefix="c" uri="http://java.sun.com/jstl/core_rt"%>
<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<title>Insert title here</title>
<script type="text/javascript"
	src="resources/scripts/jquery-3.4.1.min.js"></script>
<script type="text/javascript">
	function login() {
		var list = document.getElementsByName("type");
		for (i = 0; i < list.length; i++) {
			if (list[i].checked == true) {
				if (list[i].value == "user") {
					document.form.action = "${pageContext.request.contextPath}/home?curPage=1";
					document.form.submit();
					break;
				}
				if (list[i].value == "admin") {
					document.form.action = "${pageContext.request.contextPath}/admin";
					document.form.submit();
					break;
				}
			}
		}
	}
</script>
</head>
<body>

	<br>
	<br>
	<form name="form" method="post">
		账号：<input type="text" name="username" /> <br> <br> 密码：<input
			type="password" name="password" /> <br> <br> <input
			type="radio" name="type" value="user" checked="checked">用户 <input
			type="radio" name="type" value="admin">管理员 <br> <br>
		<input onclick=javascript:login(); type="button" value="登录" />
	</form>

	<br>

	<br>

	<a href="${pageContext.request.contextPath}/register">注册</a>

</body>
</html>