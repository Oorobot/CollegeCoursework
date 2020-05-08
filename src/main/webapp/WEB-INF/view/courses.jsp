<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
<%@taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core"%>
<%@ taglib prefix="fn" uri="http://java.sun.com/jsp/jstl/functions"%>
<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<title>Insert title here</title>
</head>
<body>
	<div class="container">

		<div>
			<c:if test="${fn:contains(category,'open') and empty terms }">
				<label style="color: blue;">开课阶段未到！！！</label>
			</c:if>
			<c:if test="${fn:contains(category,'course') }">
				<a href="${pageContext.request.contextPath}/admin/course/add">
					<button type="button" class="btn btn-success">添加</button>
				</a>
			</c:if>
			<c:if test="${!empty success }">
				<label style="color: green;">${success }</label>
			</c:if>
			<c:if test="${!empty error }">
				<label style="color: red;">错误：${error }</label>
			</c:if>
		</div>

		<table class="table">
			<thead>
				<tr>
					<th>课程号</th>
					<th>课程名</th>
					<th>学分</th>
					<th>学时</th>
					<th>学院</th>
					<th>操作</th>
				</tr>
			</thead>
			<tbody>
				<c:forEach items="${courses }" var="c">
					<tr>
						<td>${c.cno }</td>
						<td>${c.name }</td>
						<td>${c.credit }</td>
						<td>${c.hour }</td>
						<td>${academyName[c.ano] }</td>
						<c:if test="${fn:contains(category,'course') }">
							<td><a
								href="${pageContext.request.contextPath}/admin/course/${c.cno }">
									<button type="button" class="btn btn-info">修改</button>
							</a></td>
						</c:if>
						<c:if test="${fn:contains(category,'open') and !empty terms }">
							<td><a
								href="${pageContext.request.contextPath}/teacher/open/${c.cno }">
									<button type="button" class="btn btn-info">开课</button>
							</a></td>
						</c:if>
						<c:if test="${fn:contains(category,'open') and empty terms }">
							<td><a
								href="${pageContext.request.contextPath}/teacher/open/${c.cno }">
									<button type="button" class="btn btn-info" disabled>开课</button>
							</a></td>
						</c:if>
					</tr>
				</c:forEach>
			</tbody>
		</table>
	</div>

</body>
<script type="text/javascript"
	src="${pageContext.request.contextPath}/jquery-3.4.1.min.js"></script>
<script
	src="${pageContext.request.contextPath}/bootstrap/js/bootstrap.min.js"></script>

</html>