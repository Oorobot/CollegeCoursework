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
	<h2>我的留言信息</h2>
	<a href="${pageContext.request.contextPath}/home?curPage=1">返回首页</a>
	<c:if test="${empty messages }">无留言信息</c:if>
	<c:if test="${!empty messages }">
		<table>
			<tr>
				<td>商品名(点击进入商品详情页)</td>
				<td>你的留言</td>
			</tr>
			<c:forEach items="${messages }" var="m">
				<tr>
					<td><a
						href="${pageContext.request.contextPath}/home/commodity/${m.commodityId }?curPage=1">${cMap[m.commodityId] }</a></td>
					<td>${m.message }</td>
					<td><a
						href="${pageContext.request.contextPath}/myMessages/${m.messageId}">删除
					</a></td>
				</tr>
			</c:forEach>
		</table>
		<br>
		<table>
			<tr>
				<th scope="col">
					<form method="get"
						action="${pageContext.request.contextPath}/myMessages">
						<input type="hidden" name="curPage" value="1"> <input
							type="submit" value="首页">
					</form>
				</th>
				<th scope="col">
					<form method="get"
						action="${pageContext.request.contextPath}/myMessages">
						<input type="hidden" name="curPage" value="${page.prePage}">
						<input type="submit" value="上一页">
					</form>

				</th>
				<th>当前：第 ${page.currentPage}页,共${page.totalPage}页.</th>
				<th scope="col">
					<form method="get"
						action="${pageContext.request.contextPath}/myMessages">
						<input type="hidden" name="curPage" value="${page.nextPage}">
						<input type="submit" value="下一页">
					</form>

				</th>
				<th scope="col">
					<form method="get"
						action="${pageContext.request.contextPath}/myMessages">
						<input type="hidden" name="curPage" value="${page.totalPage}">
						<input type="submit" value="尾页">
					</form>
				</th>
			</tr>
		</table>
	</c:if>

</body>
</html>