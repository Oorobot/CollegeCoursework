<%@ page language="java" contentType="text/html; charset=UTF-8"
	pageEncoding="UTF-8"%>
<%@ taglib prefix="c" uri="http://java.sun.com/jstl/core_rt"%>
<%@ taglib prefix="form" uri="http://www.springframework.org/tags/form"%>
<%@ taglib prefix="fn" uri="http://java.sun.com/jsp/jstl/functions"%>
<%@ taglib prefix="fmt" uri="http://java.sun.com/jstl/fmt_rt"%>
<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<title>Insert title here</title>
</head>
<body>
	<h2>我发布的商品信息</h2>
	<a href="${pageContext.request.contextPath}/home?curPage=1">返回首页</a>


	<c:if test="${empty commodities }">无商品信息</c:if>
	<c:if test="${!empty commodities }">
		<table>
			<tr>
				<td>商品名称</td>
				<td>商品价格</td>
				<td>商品描述</td>
				<td>商品状态</td>
				<td>商品类别</td>
				<td></td>
			</tr>
			<c:forEach items="${commodities }" var="commodity">
				<tr>
					<td>${commodity.commodityName }</td>
					<td>${commodity.commodityPrice }</td>
					<td>${commodity.commodityDepict }</td>
					<c:if test="${commodity.isOrdered==true }">
						<td><a
							href="${pageContext.request.contextPath}/myCommodityOrder/${commodity.commodityId }">有订单信息</a></td>
					</c:if>
					<c:if test="${commodity.isOrdered==false }">
						<td>可交易</td>
					</c:if>
					<td>${ccMap[commodity.commodityClassId] }</td>
					<td><a
						href="${pageContext.request.contextPath}/home/commodity/${commodity.commodityId }?curPage=1">商品留言信息
					</a></td>
					<c:if test="${commodity.isOrdered==false }">
						<td><a
							href="${pageContext.request.contextPath}/myCommodities/${commodity.commodityId }">修改
						</a></td>
					</c:if>
				</tr>
				<c:if test=""></c:if>
			</c:forEach>
		</table>
		<table>
			<tr>
				<th scope="col">
					<form method="get"
						action="${pageContext.request.contextPath}/myCommodities">
						<input type="hidden" name="curPage" value="1"> <input
							type="submit" value="首页">
					</form>

				</th>
				<th scope="col">
					<form method="get"
						action="${pageContext.request.contextPath}/myCommodities">
						<input type="hidden" name="curPage" value="${page.prePage}">
						<input type="submit" value="上一页">
					</form>

				</th>
				<th>当前：第 ${page.currentPage}页,共${page.totalPage}页.</th>
				<th scope="col">
					<form method="get"
						action="${pageContext.request.contextPath}/myCommodities">
						<input type="hidden" name="curPage" value="${page.nextPage}">
						<input type="submit" value="下一页">
					</form>

				</th>
				<th scope="col">
					<form method="get"
						action="${pageContext.request.contextPath}/myCommodities">
						<input type="hidden" name="curPage" value="${page.totalPage}">
						<input type="submit" value="尾页">
					</form>

				</th>
			</tr>
		</table>
	</c:if>


</body>
</html>