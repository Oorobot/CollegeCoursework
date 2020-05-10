<%@ page language="java" contentType="text/html; charset=UTF-8"
	pageEncoding="UTF-8"%>
<%@taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core"%>
<%@ taglib prefix="fn" uri="http://java.sun.com/jsp/jstl/functions"%>
<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<title>已开课信息</title>
<meta http-equiv="X-UA-Compatible" content="IE=edge">
<meta name="viewport" content="width=device-width, initial-scale=1">
<link
	href="${pageContext.request.contextPath}/bootstrap/css/bootstrap.min.css"
	rel="stylesheet">
</head>
<body>
	<!-- 导航栏 -->
	<jsp:include page="teacher-nav.jsp"></jsp:include>

	<div class="container">
		<form class="form-inline"
			action="${pageContext.request.contextPath}/teacher/opened"
			method="get">
			<div class="form-group">
				<label class="sr-only">学期</label> <select class="form-control"
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
			<button type="submit" class="btn btn-default">查询</button>
			<c:if test="${!empty term and term.status != 0 }">
				<label>开课阶段已结束</label>
			</c:if>
			<c:if test="${!empty success }">
				<label style="color: green;">${success }</label>
			</c:if>
		</form>
		<c:if test="${empty term }">
			<label>请选择学期查询</label>
		</c:if>
		<c:if test="${!empty term and fn:length(CourseSchedules)==0 }">
			<label>暂无开课信息</label>
		</c:if>
		<c:if test="${!empty CourseSchedules }">

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
						<th>权重（平时成绩的比例）</th>
						<th>备注</th>
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
							<td>${cs.weight }%</td>
							<td style="color: red;">${cs.memo }</td>
							<!-- 修改按钮 -->
							<c:if test="${term.status == 0 }">
								<td><a
									href="${pageContext.request.contextPath}/teacher/opened/update/${cs.upk.termId }/${cs.upk.tno }/${cs.upk.cno }">
										<button type="button" class="btn btn-info">修改</button>
								</a></td>
							</c:if>
							<c:if test="${term.status != 0 }">
								<td><a
									href="${pageContext.request.contextPath}/teacher/opened/update/${cs.upk.termId }/${cs.upk.tno }/${cs.upk.cno }">
										<button type="button" class="btn btn-info" disabled>修改</button>
								</a></td>
							</c:if>
							<!-- 登分按钮 -->
							<c:if test="${term.status == 3 and cs.number != 0}">
								<td><a
									href="${pageContext.request.contextPath}/teacher/opened/score/${cs.upk.termId }/${cs.upk.tno }/${cs.upk.cno }">
										<button type="button" class="btn btn-success">登分</button>
								</a></td>
							</c:if>
							<c:if test="${term.status != 3 or cs.number == 0}">
								<td><a
									href="${pageContext.request.contextPath}/teacher/opened/score/${cs.upk.termId }/${cs.upk.tno }/${cs.upk.cno }">
										<button type="button" class="btn btn-success" disabled>登分</button>
								</a></td>
							</c:if>
							<!-- 查看学生信息 -->
							<td><a
								href="${pageContext.request.contextPath}/teacher/opened/look/${cs.upk.termId }/${cs.upk.tno }/${cs.upk.cno }">
									<button type="button" class="btn btn-info">学生信息</button>
							</a></td>
							<!-- 删除按钮 -->
							<c:if test="${cs.number == 0 }">
								<td><a
									href="${pageContext.request.contextPath}/teacher/opened/delete/${cs.upk.termId }/${cs.upk.tno }/${cs.upk.cno }">
										<button type="button" class="btn btn-danger">删除</button>
								</a></td>
							</c:if>
							<c:if test="${cs.number != 0 }">
								<td><a
									href="${pageContext.request.contextPath}/teacher/opened/delete/${cs.upk.termId }/${cs.upk.tno }/${cs.upk.cno }">
										<button type="button" class="btn btn-danger" disabled>删除</button>
								</a></td>
							</c:if>
							<!--  -->
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