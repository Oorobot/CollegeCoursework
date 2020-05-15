<%@ page language="java" contentType="text/html; charset=UTF-8"
	pageEncoding="UTF-8"%>
<%@taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core"%>
<%@ taglib prefix="fn" uri="http://java.sun.com/jsp/jstl/functions"%>
<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<title>教师登分</title>
<meta http-equiv="X-UA-Compatible" content="IE=edge">
<meta name="viewport" content="width=device-width, initial-scale=1">
<link
	href="${pageContext.request.contextPath}/bootstrap/css/bootstrap.min.css"
	rel="stylesheet">
</head>
<body>
	<!-- 导航栏 -->
	<jsp:include page="teacher-nav.jsp"></jsp:include>
	<!-- 主题内容 -->
	<div class="container">
		<div>
			<c:if test="${!empty error }">
				<label style="color: red;">${error }</label>
			</c:if>
		</div>
		<form
			action="${pageContext.request.contextPath}/teacher/opened/score/save">
			<table class="table">
				<thead>
					<tr>
						<th>学号</th>
						<th>姓名</th>
						<th>平时成绩</th>
						<th>考试成绩</th>
					</tr>
				</thead>
				<tbody>
					<c:forEach items="${electives }" var="e">
						<tr>
							<td>${e.upk.sno }</td>
							<td>${studentName[e.upk.sno] }</td>
							<td><input type="number" class="form-control"
								placeholder="0~100" name="usualScore" value="${e.usualScore }"></td>
							<td><input type="number" class="form-control"
								placeholder="0~100" name="examScore" value="${e.examScore }"></td>
							<td><input type="hidden" class="form-control" name="sno"
								value="${e.upk.sno }"></td>
						</tr>
					</c:forEach>
				</tbody>
			</table>
			<input type="hidden" name="cno" value=${upk.cno }> <input
				type="hidden" name="termId" value=${upk.termId }>
			<button type="submit" class="btn btn-default">提交</button>
		</form>
	</div>


</body>
<script type="text/javascript"
	src="${pageContext.request.contextPath}/jquery-3.4.1.min.js"></script>
<script
	src="${pageContext.request.contextPath}/bootstrap/js/bootstrap.min.js"></script>
</html>