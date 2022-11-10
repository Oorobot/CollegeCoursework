<%@ page language="java" contentType="text/html; charset=UTF-8"
	pageEncoding="UTF-8"%>
<%@taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core"%>
<%@ taglib prefix="fn" uri="http://java.sun.com/jsp/jstl/functions"%>
<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<title>学期管理</title>
</head>
</head>
<body>
	<div class="container">
		<div class="term_add">
			<button type="button" class="btn btn-primary" data-toggle="modal"
				data-target="#termModal" data-whatever="@mdo">开设新学期</button>
			<div class="modal fade" id="termModal" tabindex="-1" role="dialog"
				aria-labelledby="exampleModalLabel">
				<div class="modal-dialog" role="document">
					<div class="modal-content">
						<div class="modal-header">
							<button type="button" class="close" data-dismiss="modal"
								aria-label="Close">
								<span aria-hidden="true">&times;</span>
							</button>
							<h4 class="modal-title" id="TermLabel">添加新学期</h4>
						</div>
						<div class="modal-body">
							<form id="addTerm" method="post">
								<div class="form-group">
									<label for="term-name" class="control-label">新学期名称:</label> <input
										type="text" class="form-control" id="term-name"
										name="termName">
								</div>
							</form>
						</div>
						<div class="modal-footer">
							<button type="button" class="btn btn-default"
								data-dismiss="modal">取消</button>
							<button type="button" class="btn btn-primary" onclick="addTerm()">确定</button>
						</div>
					</div>
				</div>
			</div>
			<c:if test="${!empty success }">
				<label style="color: green;">${success }</label>
			</c:if>
			<c:if test="${!empty error }">
				<label style="color: red;">${error }</label>
			</c:if>
		</div><!-- 添加term按钮 -->
		<div class="term_table">
			<table class="table">
				<thead>
					<tr>
						<th>学期</th>
						<th>学期状态</th>
						<th>操作</th>
					</tr>
				</thead>
				<tbody>
					<c:forEach items="${terms }" var="t">
						<tr>
							<td>${t.term }</td>
							<td style="color: maroon;">${term_status[t.status] }</td>
							<td>

								<div class="btn-group">
									<button type="button" class="btn btn-warning dropdown-toggle"
										data-toggle="dropdown" aria-haspopup="true"
										aria-expanded="false">
										修改学期状态 <span class="caret"></span>
									</button>
									<ul class="dropdown-menu">
										<li><a
											href="${pageContext.request.contextPath}/admin/term/${t.id }/0">学期开始</a></li>
										<li role="separator" class="divider"></li>
										<li><a
											href="${pageContext.request.contextPath}/admin/term/${t.id }/1">选课阶段</a></li>
										<li><a
											href="${pageContext.request.contextPath}/admin/term/${t.id }/2">选课结束</a></li>
										<li role="separator" class="divider"></li>
										<li><a
											href="${pageContext.request.contextPath}/admin/term/${t.id }/3">教师评分</a></li>
										<li><a
											href="${pageContext.request.contextPath}/admin/term/${t.id }/4">学期结束</a></li>
									</ul>
								</div>
							</td>
						</tr>
					</c:forEach>
				</tbody>
			</table>
		</div>
		<!-- term表格 -->
	</div>

</body>
<script type="text/javascript"
	src="${pageContext.request.contextPath}/jquery-3.4.1.min.js"></script>
<script
	src="${pageContext.request.contextPath}/bootstrap/js/bootstrap.min.js"></script>
<script type="text/javascript">
	function addTerm() {
		var form = document.getElementById('addTerm');
		form.action = "${pageContext.request.contextPath}/admin/term"
		form.submit();
		$("#termModal").on("hidden.bs.modal", function() {
			$(this).removeData("bs.modal");
		});
	}
</script>
</html>