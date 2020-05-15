<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
<%@taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core"%>
<%@ taglib prefix="fn" uri="http://java.sun.com/jsp/jstl/functions"%>
<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<title>学院管理</title>
</head>
<body>
	<div class="container">

		<div>
			<a href="${pageContext.request.contextPath}/admin/academy/add">
				<button type="button" class="btn btn-success">添加</button>
			</a>
			<c:if test="${!empty success }">
				<label style="color: green;">${success }</label>
			</c:if>
			<c:if test="${!empty error }">
				<label style="color: red;">请重新添加：${error }</label>
			</c:if>
		</div>
		<div class="academy_table">
			<table class="table">
				<thead>
					<tr>
						<th>学院号</th>
						<th>学院名</th>
						<th>学院地址</th>
						<th>联系电话</th>
						<th></th>
					</tr>
				</thead>
				<tbody>
					<c:forEach items="${academies }" var="a">
					<tr>
						<td>${a.ano }</td>
						<td>${a.name }</td>
						<td>${a.address }</td>
						<td>${a.phone }</td>
						<td>
						<a href="${pageContext.request.contextPath}/admin/academy/${a.ano }">
						<button type="button" class="btn btn-info">修改</button>
						</a>
						</td>
					</tr>
					</c:forEach>
				</tbody>
			</table>
		</div>

	</div>

</body>

<script type="text/javascript"
	src="${pageContext.request.contextPath}/jquery-3.4.1.min.js"></script>
<script
	src="${pageContext.request.contextPath}/bootstrap/js/bootstrap.min.js"></script>

</html>