package com.elective.school.configurer;

import java.io.IOException;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.springframework.web.servlet.HandlerInterceptor;
import org.springframework.web.servlet.ModelAndView;

import com.elective.school.entity.Admin;
import com.elective.school.entity.Student;
import com.elective.school.entity.Teacher;

public class LoginInterceptor implements HandlerInterceptor {
	@Override
	public boolean preHandle(HttpServletRequest request, HttpServletResponse response, Object handler) {
		try {
			// 统一拦截
			Student student = (Student) request.getSession().getAttribute("student");
			Teacher teacher = (Teacher) request.getSession().getAttribute("teacher");
			Admin admin = (Admin) request.getSession().getAttribute("admin");
			if (student != null || teacher != null || admin != null) {
				return true;
			}
			response.sendRedirect(request.getContextPath() + "/login");
		} catch (IOException e) {
			e.printStackTrace();
		}
		return false;
	}

	/**
	 * 请求处理之后进行调用，但是在视图被渲染之前（Controller方法调用之后）
	 */
	@Override
	public void postHandle(HttpServletRequest request, HttpServletResponse response, Object handler,
			ModelAndView modelAndView) {
		//System.out.println("执行了TestInterceptor的postHandle方法");
	}

	/**
	 * 在整个请求结束之后被调用，也就是在DispatcherServlet 渲染了对应的视图之后执行（主要是用于进行资源清理工作）
	 */
	@Override
	public void afterCompletion(HttpServletRequest request, HttpServletResponse response, Object handler,
			Exception ex) {
		//System.out.println("执行了TestInterceptor的afterCompletion方法");
	}
}
