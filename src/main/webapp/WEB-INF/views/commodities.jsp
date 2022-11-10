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
			<td><a
				href="${pageContext.request.contextPath}/admin/users?curPage=1">用户信息</a></td>
			<td><a
				href="${pageContext.request.contextPath}/admin/commodityClasses?curPage=1">商品类别信息</a></td>
			<td><h4>商品信息</h4></td>
			<td><a
				href="${pageContext.request.contextPath}/admin/orders?curPage=1">订单信息</a></td>
			<td><a
				href="${pageContext.request.contextPath}/admin/messages?curPage=1">留言信息</a></td>
		</tr>
	</table>

	<form
		action="${pageContext.request.contextPath}/admin/commodities/search"
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
				<td>ID</td>
				<td>卖家</td>
				<td>商品名称</td>
				<td>商品价格</td>
				<td>商品图片</td>
				<td>商品描述</td>
				<td>商品状态</td>
				<td>商品类别</td>
				<td></td>
			</tr>
			<c:forEach items="${commodities }" var="commodity">
				<tr>
					<td>${commodity.commodityId }</td>
					<td>${ulist[commodity.userId] }</td>
					<td>${commodity.commodityName }</td>
					<td>${commodity.commodityPrice }</td>
					<td>${commodity.commodityImage }</td>
					<td>${commodity.commodityDepict }</td>
					<td>${commodity.isOrdered==true? "交易中":"可交易" }</td>
					<td>${clist[commodity.commodityClassId] }</td>
					<th><form
							action="${pageContext.request.contextPath}/admin/commodity/${commodity.commodityId}"
							method="get">
							<input type="hidden" name="curPage" value="1"> <input
								type="submit" value="查看">
						</form></th>
				</tr>
			</c:forEach>
		</table>
		<table>
			<tr>
				<th scope="col">
					<form method="get"
						action="${pageContext.request.contextPath}/admin/${adr }">
						<input type="hidden" name="curPage" value="1"><input
							type="hidden" name="search" value="${search }"> <input
							type="submit" value="首页">
					</form>
				</th>
				<th scope="col">
					<form method="get"
						action="${pageContext.request.contextPath}/admin/${adr }">
						<input type="hidden" name="curPage" value="${page.prePage}"><input
							type="hidden" name="search" value="${search }"> <input
							type="submit" value="上一页">
					</form>
				</th>
				<th>当前：第 ${page.currentPage}页,共${page.totalPage}页.</th>
				<th scope="col">
					<form method="get"
						action="${pageContext.request.contextPath}/admin/${adr }">
						<input type="hidden" name="curPage" value="${page.nextPage}"><input
							type="hidden" name="search" value="${search }"> <input
							type="submit" value="下一页">
					</form>
				</th>
				<th scope="col">
					<form method="get"
						action="${pageContext.request.contextPath}/admin/${adr }">
						<input type="hidden" name="curPage" value="${page.totalPage}"><input
							type="hidden" name="search" value="${search }"> <input
							type="submit" value="尾页">
					</form>
				</th>
			</tr>
		</table>
	</c:if>
	<br>
</body>
</html>