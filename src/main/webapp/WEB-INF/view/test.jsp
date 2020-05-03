<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%@ taglib prefix="c" uri="http://java.sun.com/jstl/core_rt"%>
<%@ taglib prefix="fn" uri="http://java.sun.com/jsp/jstl/functions"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="fmt"%>
<!--数据格式化标签库-->
<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<title>Insert title here</title>
	<link rel="stylesheet" href="layui/css/layui.css" media="all">
	<script type="text/javascript" src="layui/layui.js"></script>
	<script>
		layui.use([ 'layer', 'form' ], function() {
			var layer = layui.layer, form = layui.form;

			layer.msg('Hello LayUI');
		});
	</script>
</head>
<body>${now }


<c:if test="${empty category }">
			<div style="padding: 15px;">欢迎登录！！！</div>
		</c:if>
		${category }
		<c:if test="${fn:containsIgnoreCase(category,'term') }">
    	have a try!
    
    	</c:if>
	<fieldset class="layui-elem-field site-demo-button" style="margin-top: 30px;">
		<legend>按钮主题</legend>
		<div>
			<button type="button" class="layui-btn layui-btn-primary">原始按钮</button>
			<button type="button" class="layui-btn">默认按钮</button>
			<button type="button" class="layui-btn layui-btn-normal">百搭按钮</button>
			<button type="button" class="layui-btn layui-btn-warm">暖色按钮</button>
			<button type="button" class="layui-btn layui-btn-danger">警告按钮</button>
			<button type="button" class="layui-btn layui-btn-disabled">禁用按钮</button>
		</div>
	</fieldset>
</body>
</html>