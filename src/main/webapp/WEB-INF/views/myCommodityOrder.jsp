<%@ page language="java" contentType="text/html; charset=UTF-8"
	pageEncoding="UTF-8"%>
<%@ taglib prefix="c" uri="http://java.sun.com/jstl/core_rt"%>
<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<title>Insert title here</title>
</head>
<body>
	<h2>订单信息</h2>
	<h4>订单号 ：${order.orderNo }</h4>
	<h4>订单提交时间：${order.submitTime }</h4>
	<h4>订单总价：${order.totalPrice }</h4>
	<h3>
		订单状态 ：
		<c:if test="${order.isPay == false }">
		与买家商讨中。。。
		</c:if>
		<c:if test="${order.isPay == true && order.isOver != true }">
		请尽早发货！！！
		</c:if>
		<c:if test="${order.isOver == true }">
		交易已结束！！！
		</c:if>
	</h3>
	<c:if test="${order.isPay != false }">
		<h2>发货信息：</h2>
		<h4>
			买家：${user.name } <br>手机号码：${user.phone } <br>地址：
			${user.address }
		</h4>

		<form action="${pageContext.request.contextPath}/myCommodities">
			<input type="submit" value="返回">
		</form>
	</c:if>
</body>
</html>