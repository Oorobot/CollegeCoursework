<%@ page language="java" contentType="text/html; charset=UTF-8"
	pageEncoding="UTF-8"%>
<%@ taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core" %>
<!DOCTYPE html>
<html>
<head>
	<meta charset="UTF-8">
	<title>登录</title>
	<meta http-equiv="X-UA-Compatible" content="IE=edge">
	<meta name="viewport" content="width=device-width, initial-scale=1">
	<link href="bootstrap/css/bootstrap.min.css" rel="stylesheet">
<style type="text/css">
.myradio {
	text-align: center;
}
</style>

</head>
<body>
	<form name="form" method="post">
		<div class="modal-dialog" style="margin-top: 10%;">
			<div class="modal-content">
				<div class="modal-header">
					<h4 class="modal-title text-center" id="myModalLabel">登录</h4>
				</div>
				<div class="modal-body" id="model-body">
					<div class="form-group">
						<input type="text" class="form-control" placeholder="用户名"
							autocomplete="off" name="username">
					</div>
					<div class="form-group">
						<input type="password" class="form-control" placeholder="密码"
							autocomplete="off" name="password">
					</div>
					<div class="myradio">
						<label class="radio-inline">
							<input type="radio" name="type" value="student" checked="checked">学生
						</label>
						<label class="radio-inline">
							<input type="radio" name="type" value="teacher">教师
						</label>
						<label class="radio-inline">
							<input type="radio" name="type" value="admin">管理员
						</label>

					</div>
					<div>
						<div class="msg" style="border: none;text-align: center; color: red;">
							<c:if test="${!empty msg }">${msg }</c:if>
						</div>
					</div>

				</div>
				<div class="modal-footer">
					<div class="form-group">
						<button type="button" class="btn btn-primary form-control"
							onclick=javascript:login();>登录</button>
					</div>
				</div>
			</div>
		</div>
	</form>

</body>




<script type="text/javascript"  src="jquery-3.4.1.min.js"></script>
<script src="bootstrap/js/bootstrap.min.js"></script>
<script type="text/javascript">
	function login() {
		var list = document.getElementsByName("type");
		for (i = 0; i < list.length; i++) {
			if (list[i].checked == true) {
				if (list[i].value == "student") {
					document.form.action = "${pageContext.request.contextPath}/student";
					document.form.submit();
					break;
				}
				if (list[i].value == "teacher") {
					document.form.action = "${pageContext.request.contextPath}/teacher";
					document.form.submit();
					break;
				}
				if (list[i].value == "admin") {
					document.form.action = "${pageContext.request.contextPath}/admin";
					document.form.submit();
					break;
				}
			}
		}
	}
</script>
</html>