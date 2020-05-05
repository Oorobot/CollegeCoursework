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
			<a href="${pageContext.request.contextPath}/admin/teacher/add">
				<button type="button" class="btn btn-success">添加</button>
			</a>
			<c:if test="${!empty success }">
				<label style="color: green;">${success }</label>
			</c:if>
			<c:if test="${!empty error }">
				<label style="color: red;">请重新添加：${error }</label>
			</c:if>
		</div>

		<table class="table">
			<thead>
				<tr>
					<th>工号</th>
					<th>姓名</th>
					<th>性别</th>
					<th>出生日期</th>
					<th>职称</th>
					<th>工资</th>
					<th>学院</th>
					<th>操作</th>
				</tr>
			</thead>
			<tbody>
				<c:forEach items="${teachers }" var="t">
					<tr>
						<td>${t.tno }</td>
						<td>${t.name }</td>
						<td>${t.sex == false ? "男" : "女" }</td>
						<td>${t.birthday }</td>
						<td>${t.position }</td>
						<td>${t.salary }</td>
						<td>${academyName[t.ano] }</td>
						<td><a
							href="${pageContext.request.contextPath}/admin/teacher/${t.tno }">
								<button type="button" class="btn btn-warning">初始化密码</button>
						</a></td>
						<td><a
							href="${pageContext.request.contextPath}/admin/teacher/update/${t.tno }">
								<button type="button" class="btn btn-info">修改</button>
						</a></td>
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
<script type="text/javascript">
$(".btn-warning").click(function(){
	alert("密码初始化为：123456");
});

</script>
</html>