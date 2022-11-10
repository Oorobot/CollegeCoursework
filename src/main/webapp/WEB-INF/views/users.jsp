<%@ page language="java" contentType="text/html; charset=UTF-8"
	pageEncoding="UTF-8"%>
<%@ taglib prefix="c" uri="http://java.sun.com/jstl/core_rt"%>
<%@ taglib prefix="form" uri="http://www.springframework.org/tags/form"%>
<%@ taglib prefix="fn" uri="http://java.sun.com/jsp/jstl/functions"%>
<%@ taglib prefix="fmt" uri="http://java.sun.com/jstl/fmt_rt"%>

<!DOCTYPE html>
<html>
<head>
<meta charset="ISO-8859-1">
<title>Insert title here</title>

<style type="text/css">
table {
	width: 90%;
	border-collapse: collapse;
	margin: 0 auto;
	text-align: center;
}

table td, table th {
	border: 1px solid #cad9ea;
	color: #666;
	height: 30px;
}

table thead th {
	background-color: #CCE8EB;
	width: 100px;
}

table tr:nth-child(odd) {
	background: #fff;
}

table tr:nth-child(even) {
	background: #F5FAFA;
}
</style>

</head>
<body>
	<table>
		<tr>
			<td><h4>用户信息</h4></td>
			<td><a
				href="${pageContext.request.contextPath}/admin/commodityClasses?curPage=1">商品类别信息</a></td>
			<td><a
				href="${pageContext.request.contextPath}/admin/commodities?curPage=1">商品信息</a></td>
			<td><a href="${pageContext.request.contextPath}/admin/orders?curPage=1">订单信息</a></td>
			<td><a href="${pageContext.request.contextPath}/admin/messages?curPage=1">留言信息</a></td>
		</tr>
	</table>
	<br>
	<c:if test="${empty users }">目前暂无任何用户信息</c:if>
	<c:if test="${!empty users }">
		<table class="table">
			<tr>
				<td>ID</td>
				<td>账号</td>
				<td>密码</td>
				<td>用户名</td>
				<td>性别</td>
				<td>手机号码</td>
				<td>地址</td>
			</tr>
			<c:forEach items="${users}" var="user" begin="0"
				end="${fn:length(users) - 1}">
				<tr>
					<td>${user.userId}</td>
					<td>${user.username}</td>
					<td>${user.password}</td>
					<td>${user.name}</td>
					<td>${user.sex == null ? " " : user.sex == true ? "男" : "女"}</td>
					<td>${user.phone}</td>
					<td>${user.address}</td>
					<th><form
							action="${pageContext.request.contextPath}/admin/users/${user.userId}"
							method="post">
							<input type="submit" value="删除">
						</form></th>
				</tr>
			</c:forEach>
		</table>
		<br>
		<table>
			<tr>
				<th scope="col">
					<form method="get"
						action="${pageContext.request.contextPath}/admin/users">
						<input type="hidden" name="curPage" value="1"> <input
							type="submit" value="首页">
					</form>

				</th>
				<th scope="col">
					<form method="get"
						action="${pageContext.request.contextPath}/admin/users">
						<input type="hidden" name="curPage" value="${page.prePage}">
						<input type="submit" value="上一页">
					</form>

				</th>
				<th>当前：第 ${page.currentPage}页,共${page.totalPage}页.</th>
				<th scope="col">
					<form method="get"
						action="${pageContext.request.contextPath}/admin/users">
						<input type="hidden" name="curPage" value="${page.nextPage}">
						<input type="submit" value="下一页">
					</form>

				</th>
				<th scope="col">
					<form method="get"
						action="${pageContext.request.contextPath}/admin/users">
						<input type="hidden" name="curPage" value="${page.totalPage}">
						<input type="submit" value="尾页">
					</form>

				</th>
			</tr>
		</table>
	</c:if>
	<br>

</body>
</html>