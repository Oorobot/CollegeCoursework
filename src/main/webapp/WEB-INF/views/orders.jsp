<%@ page language="java" contentType="text/html; charset=UTF-8"
	pageEncoding="UTF-8"%>
<%@ taglib prefix="c" uri="http://java.sun.com/jstl/core_rt"%>
<%@ taglib prefix="form" uri="http://www.springframework.org/tags/form"%>
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
			<td><a
				href="${pageContext.request.contextPath}/admin/users?curPage=1">用户信息</a></td>
			<td><a
				href="${pageContext.request.contextPath}/admin/commodityClasses?curPage=1">商品类别信息</a></td>
			<td><a
				href="${pageContext.request.contextPath}/admin/commodities?curPage=1">商品信息</a></td>
			<td><h4>订单信息</h4></td>
			<td><a href="${pageContext.request.contextPath}/admin/messages?curPage=1">留言信息</a></td>
		</tr>
	</table>

	<c:if test="${empty orders}">无订单信息</c:if>
	<c:if test="${!empty orders}">
		<table>
			<tr>
				<td>ID</td>
				<td>订单号</td>
				<td>卖家</td>
				<td>商品名</td>
				<td>买家</td>
				<td>商讨状态</td>
				<td>交货状态</td>
				<td>订单提交时间</td>
				<td>订单总价</td>
			</tr>
			<c:forEach items="${orders}" var="o">
				<tr>
					<td>${o.orderId }</td>
					<td>${o.orderNo }</td>
					<td>${sellerMap[o.commodityId] }</td>
					<td>${cMap[o.commodityId] }</td>
					<td>${buyerMap[o.userId] }</td>
					<td>${o.isPay == false ? "商讨中" : "商讨结束" }</td>
					<td>${o.isOver == false ? "交货中" : "交货结束"}</td>
					<td>${o.submitTime }</td>
					<td>${o.totalPrice }</td>
					<th><form
							action="${pageContext.request.contextPath}/admin/orders/${o.orderId}"
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
						action="${pageContext.request.contextPath}/admin/orders">
						<input type="hidden" name="curPage" value="1"> <input
							type="submit" value="首页">
					</form>
				</th>
				<th scope="col">
					<form method="get"
						action="${pageContext.request.contextPath}/admin/orders">
						<input type="hidden" name="curPage" value="${page.prePage}">
						<input type="submit" value="上一页">
					</form>

				</th>
				<th>当前：第 ${page.currentPage}页,共${page.totalPage}页.</th>
				<th scope="col">
					<form method="get"
						action="${pageContext.request.contextPath}/admin/orders">
						<input type="hidden" name="curPage" value="${page.nextPage}">
						<input type="submit" value="下一页">
					</form>

				</th>
				<th scope="col">
					<form method="get"
						action="${pageContext.request.contextPath}/admin/orders">
						<input type="hidden" name="curPage" value="${page.totalPage}">
						<input type="submit" value="尾页">
					</form>
				</th>
			</tr>
		</table>
	</c:if>

</body>
</html>