<%@ page language="java" contentType="text/html; charset=UTF-8"
	pageEncoding="UTF-8"%>
<%@ taglib prefix="c" uri="http://java.sun.com/jstl/core_rt"%>
<%@ taglib prefix="fn" uri="http://java.sun.com/jsp/jstl/functions"%>
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
	<a>你好，${name }.</a>
	<a href="${pageContext.request.contextPath}/logout">退出</a>
	<br>
	<a href="${pageContext.request.contextPath}/home?curPage=1">首页</a>
	<a href="${pageContext.request.contextPath}/userInfo">个人资料</a>
	<a href="${pageContext.request.contextPath}/myCommodities?curPage=1">我的商品</a>
	<a href="${pageContext.request.contextPath}/myOrders?curPage=1">我的订单</a>
	<a href="${pageContext.request.contextPath}/myMessages?curPage=1">我的留言</a>
	<a href="${pageContext.request.contextPath}/newCommodity">发布商品</a>
	<br>
	<c:forEach items="${clist }" var="cc">
		<a href="${pageContext.request.contextPath}/home/${cc.key}?curPage=1">${cc.value}</a>
	</c:forEach>

	<form action="${pageContext.request.contextPath}/home/search"
		method="get">
		<input type="search" name="search"> <input type="hidden"
			name="curPage" value="1"> <input type="submit" value="搜索">
	</form>
	<c:if test="${fn:length(search) > 0 }">
	         搜索 “ ${search } ”的结果如下：
	</c:if>

	<c:if test="${empty commodities }">无商品信息</c:if>
	<c:if test="${!empty commodities }">
		<table>
			<tr>
				<td>卖家</td>
				<td>商品名称</td>
				<td>商品状态</td>
				<td>商品类别</td>
				<td>商品价格</td>
				<td></td>
			</tr>
			<c:forEach items="${commodities }" var="commodity">
				<tr>
					<td>${ulist[commodity.userId] }</td>
					<td>${commodity.commodityName }</td>
					<td>${commodity.isOrdered==true? "交易中":"可交易" }</td>
					<td>${commodity.commodityPrice }</td>
					<td>${clist[commodity.commodityClassId] }</td>
					<c:if test="${commodity.isOrdered==false }">
						<th><form
								action="${pageContext.request.contextPath}/home/commodity/${commodity.commodityId}"
								method="get">
								<input type="hidden" name="curPage" value="1"> <input
									type="submit" value="查看">
							</form></th>
					</c:if>
				</tr>
			</c:forEach>
		</table>
		<table>
			<tr>
				<th scope="col">
					<form method="get"
						action="${pageContext.request.contextPath}/${adr }">
						<input type="hidden" name="curPage" value="1"> <input
							type="hidden" name="search" value="${search }"><input
							type="submit" value="首页">
					</form>
				</th>
				<th scope="col">
					<form method="get"
						action="${pageContext.request.contextPath}/${adr }">
						<input type="hidden" name="curPage" value="${page.prePage}">
						<input type="hidden" name="search" value="${search }"> <input
							type="submit" value="上一页">
					</form>
				</th>
				<th>当前：第 ${page.currentPage}页,共${page.totalPage}页.</th>
				<th scope="col">
					<form method="get"
						action="${pageContext.request.contextPath}/${adr }">
						<input type="hidden" name="curPage" value="${page.nextPage}">
						<input type="hidden" name="search" value="${search }"> <input
							type="submit" value="下一页">
					</form>
				</th>
				<th scope="col">
					<form method="get"
						action="${pageContext.request.contextPath}/${adr }">
						<input type="hidden" name="curPage" value="${page.totalPage}">
						<input type="hidden" name="search" value="${search }"> <input
							type="submit" value="尾页">
					</form>
				</th>
			</tr>
		</table>
	</c:if>



</body>
</html>