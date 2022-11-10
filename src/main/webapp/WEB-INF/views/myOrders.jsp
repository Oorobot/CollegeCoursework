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
	<h2>我的订单信息</h2>
	<a href="${pageContext.request.contextPath}/home?curPage=1">返回首页</a>
	<c:if test="${empty orders}">无订单信息</c:if>
	<c:if test="${!empty orders}">
		<table>
			<tr>
				<td>订单号</td>
				<td>卖家</td>
				<td>商品名</td>
				<td>商品状态</td>
				<td>订单提交时间</td>
				<td>订单总价</td>
			</tr>
			<c:forEach items="${orders}" var="o">
				<tr>
					<td>${o.orderNo }</td>
					<td>${sellerMap[o.commodityId] }</td>
					<td>${cMap[o.commodityId] }</td>
					<td><c:if test="${o.isPay == false }">商讨中</c:if> <c:if
							test="${o.isPay == true }">
						${o.isOver == false ? "交货中" : "订单完成"}
					</c:if></td>
					<td>${o.submitTime }</td>
					<td>${o.totalPrice }</td>
					<th><form
							action="${pageContext.request.contextPath}/home/commodity/${o.commodityId }"
							method="get">
							<input type="hidden" name="curPage" value="1">
							<input type="submit" value="查看商品详情及留言信息">
						</form></th>
					<th><form
							action="${pageContext.request.contextPath}/myOrders/${o.orderId }"
							method="get">
							<input type="submit" value="刪除">
						</form></th>
					<c:if test="${o.isPay == false }">
						<th><form
								action="${pageContext.request.contextPath}/myOrders/pay"
								method="post">
								<input type="hidden" name="Id" value="${o.orderId }"> <input
									type="submit" value="请求发货">
							</form></th>
					</c:if>
					<c:if test="${o.isPay == true && o.isOver != true}">
						<th><form
								action="${pageContext.request.contextPath}/myOrders/over"
								method="post">
								<input type="hidden" name="Id" value="${o.orderId }"> <input
									type="submit" value="确认收货 ">
							</form>
							<p>请确保您的个人信息中手机号码以及地址正确。</p></th>
					</c:if>
				</tr>
			</c:forEach>
		</table>
		<br>
		<table>
			<tr>
				<th scope="col">
					<form method="get"
						action="${pageContext.request.contextPath}/myOrders">
						<input type="hidden" name="curPage" value="1"> <input
							type="submit" value="首页">
					</form>
				</th>
				<th scope="col">
					<form method="get"
						action="${pageContext.request.contextPath}/myOrders">
						<input type="hidden" name="curPage" value="${page.prePage}">
						<input type="submit" value="上一页">
					</form>

				</th>
				<th>当前：第 ${page.currentPage}页,共${page.totalPage}页.</th>
				<th scope="col">
					<form method="get"
						action="${pageContext.request.contextPath}/myOrders">
						<input type="hidden" name="curPage" value="${page.nextPage}">
						<input type="submit" value="下一页">
					</form>

				</th>
				<th scope="col">
					<form method="get"
						action="${pageContext.request.contextPath}/myOrders">
						<input type="hidden" name="curPage" value="${page.totalPage}">
						<input type="submit" value="尾页">
					</form>
				</th>
			</tr>
		</table>
	</c:if>

</body>
</html>