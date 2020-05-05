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
		<label>学期：${term },	课程名：${courseName },	教师：${teacherName }</label>
		<br><label>共${numOfElectives }名学生</label>
		<table class="table">
				<thead>
					<tr>
						<th>学号</th>
						<th>姓名</th>
						<th>平时成绩</th>
						<th>考试成绩</th>
						<th>总成绩</th>
					</tr>
				</thead>
				<tbody>
					<c:forEach items="${electives }" var="e">
						<tr>
							<td>${e.upk.sno }</td>
							<td>${studentName[e.upk.sno] }</td>
							<td>${e.usualScore }</td>
							<td>${e.examScore }</td>
							<td>${e.totalScore }</td>
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