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
</head>
<body>
	<input type="image"
		src="${pageContext.request.contextPath}/${commodity.commodityImage}"
		width="200px" height="250px">
	<br> 商品名称： ${commodity.commodityName }
	<br> 商品类别： ${commodityClassName }
	<br> 商品描述： ${commodity.commodityDepict }
	<br> 商品价格： ${commodity.commodityPrice } 元
	<br> 卖家： ${sellerName }
	<br>
	<%
		String str = "home";
		request.setAttribute("str", str);
	%>
	<table>
		<tr>
			<c:if test="${commodity.isOrdered==false&&fn:contains(adr,str) }">
				<th><form action="${pageContext.request.contextPath}/home/buy"
						method="POST">
						<input type="hidden" name="c_cId"
							value="${commodity.commodityId }"> <input type="hidden"
							name="c_price" value="${commodity.commodityPrice }"> <input
							type="submit" value="购买">
					</form></th>
			</c:if>
			<th><form
					action="${pageContext.request.contextPath}/${returnAdr }">
					<input type="submit" value="返回首页">
				</form></th>
		<tr>
	</table>
	<c:if test="${!empty messages }">
		<c:forEach items="${messages }" var="m">
			<br>${uMap[m.userId] } : ${m.message }
	</c:forEach>
		<table>
			<tr>
				<th scope="col">
					<form method="get"
						action="${pageContext.request.contextPath}/${adr }">
						<input type="hidden" name="curPage" value="1"> <input
							type="submit" value="首页">
					</form>
				</th>
				<th scope="col">
					<form method="get"
						action="${pageContext.request.contextPath}/${adr }">
						<input type="hidden" name="curPage" value="${page.prePage}">
						<input type="submit" value="上一页">
					</form>
				</th>
				<th>当前：第 ${page.currentPage}页,共${page.totalPage}页.</th>
				<th scope="col">
					<form method="get"
						action="${pageContext.request.contextPath}/${adr }">
						<input type="hidden" name="curPage" value="${page.nextPage}">
						<input type="submit" value="下一页">
					</form>
				</th>
				<th scope="col">
					<form method="get"
						action="${pageContext.request.contextPath}/${adr }">
						<input type="hidden" name="curPage" value="${page.totalPage}">
						<input type="submit" value="尾页">
					</form>
				</th>
			</tr>
		</table>
	</c:if>
	<br>

	<c:if test="${fn:contains(adr,str) }">
		<form
			action="${pageContext.request.contextPath}/home/commodity/${commodity.commodityId }?curPage=1"
			method="post">
			<input type="text" name="mge"> <input type="submit"
				value="留言">
		</form>
	</c:if>

</body>
</html>