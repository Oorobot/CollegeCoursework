<%@ page language="java" contentType="text/html; charset=UTF-8"
	pageEncoding="UTF-8"%>
<%@ taglib prefix="c" uri="http://java.sun.com/jstl/core_rt"%>
<%@ taglib prefix="fn" uri="http://java.sun.com/jsp/jstl/functions"%>
<%@ taglib prefix="form" uri="http://www.springframework.org/tags/form"%>
<%@ taglib prefix="fmt" uri="http://java.sun.com/jstl/fmt_rt"%>
<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
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
			<td><a href="${pageContext.request.contextPath}/admin/users?curPage=1">用户信息</a></td>
			<td><h4>商品类别信息</h4></td>
			<td><a
				href="${pageContext.request.contextPath}/admin/commodities?curPage=1">商品信息</a></td>
			<td><a href="${pageContext.request.contextPath}/admin/orders?curPage=1">订单信息</a></td>
			<td><a href="${pageContext.request.contextPath}/admin/messages?curPage=1">留言信息</a></td>
		</tr>
	</table>
	<a href="${pageContext.request.contextPath}/admin/commodityClass">添加新的商品类别</a>
	<c:if test="${empty commodityClasses }">无商品类信息</c:if>
	<c:if test="${!empty commodityClasses }">
		<table>
			<tr>
				<td>商品类名</td>
				<td>商品数量</td>
			</tr>
			<c:forEach items="${commodityClasses }" var="cc">
				<tr>
					<td>${cc.commodityClassName }</td>
					<td>${cclist[cc.commodityClassId] }</td>
					<th><form
							action="${pageContext.request.contextPath}/admin/commodityClass/${cc.commodityClassId}"
							method="get">
							<input type="submit" value="修改">
						</form></th>
					<th><form
							action="${pageContext.request.contextPath}/admin/commodityClasses/${cc.commodityClassId}"
							method="get">
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
						action="${pageContext.request.contextPath}/admin/commodityClasses">
						<input type="hidden" name="curPage" value="1"> <input
							type="submit" value="首页">
					</form>

				</th>
				<th scope="col">
					<form method="get"
						action="${pageContext.request.contextPath}/admin/commodityClasses">
						<input type="hidden" name="curPage" value="${page.prePage}">
						<input type="submit" value="上一页">
					</form>

				</th>
				<th>当前：第 ${page.currentPage}页,共${page.totalPage}页.</th>
				<th scope="col">
					<form method="get"
						action="${pageContext.request.contextPath}/admin/commodityClasses">
						<input type="hidden" name="curPage" value="${page.nextPage}">
						<input type="submit" value="下一页">
					</form>

				</th>
				<th scope="col">
					<form method="get"
						action="${pageContext.request.contextPath}/admin/commodityClasses">
						<input type="hidden" name="curPage" value="${page.totalPage}">
						<input type="submit" value="尾页">
					</form>

				</th>
			</tr>
		</table>
	</c:if>
</body>
</html>