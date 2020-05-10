<%@ page language="java" contentType="text/html; charset=UTF-8"
	pageEncoding="UTF-8"%>
<%@taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core"%>
<%@ taglib prefix="fn" uri="http://java.sun.com/jsp/jstl/functions"%>
<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<title>选课</title>
<meta http-equiv="X-UA-Compatible" content="IE=edge">
<meta name="viewport" content="width=device-width, initial-scale=1">
<link
	href="${pageContext.request.contextPath}/bootstrap/css/bootstrap.min.css"
	rel="stylesheet">
</head>
<body>
	<!-- 导航栏 -->
	<jsp:include page="student-nav.jsp"></jsp:include>
	<div class="container">
		<!-- 查询窗口 -->
		<c:if test="${!empty terms }">
			<form class="form-inline"
				action="${pageContext.request.contextPath}/student/select"
				method="get">
				<div class="form-group">
					<label for="termId">学期&nbsp;</label> <select class="form-control"
						name="termId">
						<c:forEach items="${terms }" var="t">
							<c:if test="${!empty term and term.id==t.id }">
								<option value="${t.id }" selected>${t.term }</option>
							</c:if>
							<c:if test="${empty term or term.id!=t.id }">
								<option value="${t.id }">${t.term }</option>
							</c:if>
						</c:forEach>
					</select>
				</div>
				<br>
				<div class="form-group">
					<label for="cno">课程号</label> <input type="number"
						class="form-control" id="cno" placeholder="0830xxxx" name="cno"
						value="${cno }">
				</div>
				<div class="form-group">
					<label for="cname">课程名</label> <input type="text"
						class="form-control" id="cname" placeholder="课程名" name="cname"
						value="${cname }">
				</div>
				<br>
				<div class="form-group">
					<label for="cno">教师号</label> <input type="number"
						class="form-control" id="cno" placeholder="01xx" name="tno"
						value="${tno }">
				</div>
				<div class="form-group">
					<label for="tname">教师名</label> <input type="text"
						class="form-control" id="tname" placeholder="教师名字" name="tname"
						value="${tname }">
				</div>
				<br>
				<button type="submit" class="btn btn-info">查询</button>
				<!-- 选课反馈结果 -->
				<c:if test="${!empty success }">
					<label style="color: green;">${success }</label>
				</c:if>
				<c:if test="${!empty error }">
					<label style="color: red;">${error }</label>
				</c:if>
			</form>
		</c:if>
		<c:if test="${empty terms }">
			<h1>
				选课时间未到！！！<small>选课时间未到！！！</small>
			</h1>
		</c:if>
		<c:if test="${!empty term and fn:length(CourseSchedules)==0 }">
			<label>暂无课程信息</label>
		</c:if>
		<c:if test="${!empty CourseSchedules }">
			<!-- 主题内容 -->
			<table class="table">
				<thead>
					<tr>
						<th>课程号</th>
						<th>课程</th>
						<th>工号</th>
						<th>教师名</th>
						<th>人数</th>
						<th>上课时间</th>
						<th>上课教室</th>
						<th>操作</th>
					</tr>
				</thead>
				<tbody>
					<c:forEach items="${CourseSchedules }" var="cs">
						<tr>
							<td>${cs.upk.cno }</td>
							<td>${courseName[cs.upk.cno] }</td>
							<td>${cs.upk.tno }</td>
							<td>${teacherName[cs.upk.tno] }</td>
							<td style="color: blue;">${cs.number }/${cs.num }</td>
							<td>${cs.time }</td>
							<td>${cs.classroom }</td>
							<c:if test="${cs.number >= cs.num }">
								<td><a
									href="${pageContext.request.contextPath}/student/select/${cs.upk.termId }/${cs.upk.tno }/${cs.upk.cno }">
										<button type="button" class="btn btn-success" disabled>选课</button>
								</a></td>
							</c:if>
							<c:if test="${cs.number < cs.num }">
								<td><form
										action="${pageContext.request.contextPath}/student/select/${cs.upk.termId }/${cs.upk.tno }/${cs.upk.cno }"
										method="get">
										<input type="hidden" name="search" value="${cno }"> <input
											type="hidden" name="search" value="${cname }"> <input
											type="hidden" name="search" value="${tno }"> <input
											type="hidden" name="search" value="${tname }">
										<button type="submit" class="btn btn-success">选课</button>
									</form></td>
							</c:if>
						</tr>
					</c:forEach>
				</tbody>
			</table>
		</c:if>
	</div>


</body>
<script type="text/javascript"
	src="${pageContext.request.contextPath}/jquery-3.4.1.min.js"></script>
<script
	src="${pageContext.request.contextPath}/bootstrap/js/bootstrap.min.js"></script>
</html>