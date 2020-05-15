<%@ page language="java" contentType="text/html; charset=UTF-8"
	pageEncoding="UTF-8"%>
<%@taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core"%>
<%@ taglib prefix="fn" uri="http://java.sun.com/jsp/jstl/functions"%>
<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<title>课程中学生信息</title>
</head>
<body>
	<div class="container">
		<label>学期：${term }, 课程名：${courseName }, 教师：${teacherName }</label> <br>
		<label>共${numOfElectives }名学生</label>
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
						<c:if test="${empty e.usualScore }">
							<td>无成绩</td>
						</c:if>
						<c:if test="${!empty e.usualScore }">
							<td>${e.usualScore }</td>
						</c:if>
						<c:if test="${empty e.examScore }">
							<td>无成绩</td>
						</c:if>
						<c:if test="${!empty e.examScore }">
							<td>${e.examScore }</td>
						</c:if>
						<c:if test="${empty e.totalScore }">
							<td>无成绩</td>
						</c:if>
						<c:if test="${!empty e.totalScore }">
							<td>${e.totalScore }</td>
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