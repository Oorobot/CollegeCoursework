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
		<form class="form-inline"
			action="${pageContext.request.contextPath}/admin/cs" method="get">
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
			<c:if test="${!empty term and term.status != 0 }"><label>开课阶段已结束</label></c:if>
			<c:if test="${!empty term and term.status == 0 }"><label style="color: blue;">开课阶段中</label>
				<a href="${pageContext.request.contextPath}/admin/cs/${term.id}">
					<button type="button" class="btn btn-success">开始选课阶段</button>
				</a>
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
							<td>${cs.num }</td>
							<td>${cs.time }</td>
							<td>${cs.classroom }</td>
							<td>${cs.weight }</td>
							<td style="color: red;">${cs.memo }</td>
							<c:if test="${term.status == 0 }">
								<td>
									<button type="button" class="btn btn-primary" id="memoButton"
										data-toggle="modal" data-target="#csModal"
										value="/${cs.upk.termId }/${cs.upk.tno }/${cs.upk.cno }">备注</button>
								</td>
							</c:if>
							<td><a
								href="${pageContext.request.contextPath}/admin/cs/look/${cs.upk.termId }/${cs.upk.tno }/${cs.upk.cno }">
									<button type="button" class="btn btn-info">查看选课信息</button>
							</a></td>
						</tr>
					</c:forEach>
				</tbody>
			</table>
		</c:if>
	</div>

	<!-- modal -->
	<div class="modal fade" id="csModal" tabindex="-1" role="dialog"
		aria-labelledby="csModalLabel">
		<div class="modal-dialog" role="document">
			<div class="modal-content">
				<div class="modal-header">
					<button type="button" class="close" data-dismiss="modal"
						aria-label="Close">
						<span aria-hidden="true">&times;</span>
					</button>
					<h4 class="modal-title" id="exampleModalLabel">请输入备注信息</h4>
				</div>
				<div class="modal-body">
					<form id="memoForm">
						<div class="form-group">
							<label for="recipient-name" class="control-label">备注:</label>
							<textarea class="form-control" rows="3" name="memo"></textarea>
						</div>
					</form>
				</div>
				<div class="modal-footer" style="text-align: center;">
					<button type="button" class="btn btn-default" data-dismiss="modal">关闭</button>
					<button type="button" class="btn btn-primary" onclick="memo()">提交</button>
				</div>
			</div>
		</div>
	</div>



</body>
<script type="text/javascript"
	src="${pageContext.request.contextPath}/jquery-3.4.1.min.js"></script>
<script
	src="${pageContext.request.contextPath}/bootstrap/js/bootstrap.min.js"></script>
<script type="text/javascript">
	
	function memo() {
		var form = document.getElementById('memoForm');
		var button = document.getElementById('memoButton');
		form.action = "${pageContext.request.contextPath}/admin/cs"
				+ button.value;
		form.submit();
		$("#termModal").on("hidden.bs.modal", function() {
			$(this).removeData("bs.modal");
		});
	}
</script>

</html>