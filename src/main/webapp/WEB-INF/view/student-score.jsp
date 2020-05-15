<%@ page language="java" contentType="text/html; charset=UTF-8"
	pageEncoding="UTF-8"%>
<%@taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core"%>
<%@ taglib prefix="fn" uri="http://java.sun.com/jsp/jstl/functions"%>
<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<title>学生成绩</title>
<meta http-equiv="X-UA-Compatible" content="IE=edge">
<meta name="viewport" content="width=device-width, initial-scale=1">
<link
	href="${pageContext.request.contextPath}/bootstrap/css/bootstrap.min.css"
	rel="stylesheet">
</head>
<body>
	<!-- 导航栏 -->
	<jsp:include page="student-nav.jsp"></jsp:include>
	<!-- 搜索栏 -->
	<div class="container">
		<form class="form-inline"
			action="${pageContext.request.contextPath}/student/score"
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
					<c:if test="${term.id == -1 }">
						<option value="-1" selected>全部</option>
					</c:if>
					<c:if test="${term.id != -1 }">
						<option value="-1">全部</option>
					</c:if>
				</select>
			</div>
			<button type="submit" class="btn btn-default">查询</button>
			<c:if test="${empty term }">
				<label>请选择查询</label>
			</c:if>
		</form>
		<c:if test="${!empty term and term.status != 4 and term.id != -1 }">
			<h1>
				成绩尚未公布！！！<small>成绩尚未公布！！！</small>
			</h1>
		</c:if>
		<c:if test="${term.status == 4 }">
			<!-- 主体内容 -->
			<c:if test="${empty electives }">
				<h4>无成绩信息</h4>
			</c:if>
			<c:if test="${!empty electives }">
				<table class="table">
					<thead>
						<tr>
							<th>课程号</th>
							<th>课程名</th>
							<th>学分</th>
							<th>成绩</th>
						</tr>
					</thead>
					<tbody>
						<c:forEach items="${electives }" var="e">
							<tr>
								<td>${e.upk.cno }</td>
								<td>${courseName[e.upk.cno] }</td>
								<td>${courseCredit[e.upk.cno] }</td>
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
			</c:if>
		</c:if>
		<!-- 选择全部时，显示成绩 -->
		<c:if test="${term.id == -1 }">
			<c:if test="${empty termsOfElectives }">
				<h4>无成绩信息</h4>
			</c:if>
			<c:if test="${!empty termsOfElectives }">
				<table class="table">
					<thead>
						<tr>
							<th>学期</th>
							<th>课程号</th>
							<th>课程名</th>
							<th>学分</th>
							<th>成绩</th>
						</tr>
					</thead>
					<c:forEach items="${termsOfElectives }" var="te">
						<tbody>
							<c:if test="${te.status != 4 }">
								<tr>
									<td>${te.term }</td>
									<td style="color: blue;">该学期课程尚未公布</td>
								</tr>
							</c:if>
							<c:if test="${te.status == 4 }">
								<c:forEach items="${electives }" var="e">
									<c:if test="${e.upk.termId == te.id }">
										<tr>
											<td>${te.term }</td>
											<td>${e.upk.cno }</td>
											<td>${courseName[e.upk.cno] }</td>
											<td>${courseCredit[e.upk.cno] }</td>
											<c:if test="${empty e.totalScore }">
												<td>无成绩</td>
											</c:if>
											<c:if test="${!empty e.totalScore }">
												<td>${e.totalScore }</td>
											</c:if>
										</tr>
									</c:if>
								</c:forEach>
							</c:if>
							<tr>
								<td></td>
								<td></td>
								<td></td>
								<td></td>
								<td></td>
							</tr>
						</tbody>
					</c:forEach>
				</table>
			</c:if>
		</c:if>
	</div>

</body>
<script type="text/javascript"
	src="${pageContext.request.contextPath}/jquery-3.4.1.min.js"></script>
<script
	src="${pageContext.request.contextPath}/bootstrap/js/bootstrap.min.js"></script>
</html>