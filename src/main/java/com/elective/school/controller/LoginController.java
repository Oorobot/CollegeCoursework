package com.elective.school.controller;

import java.util.Map;

import javax.servlet.http.HttpSession;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.SessionAttributes;
import org.springframework.web.bind.support.SessionStatus;

import com.elective.school.entity.Admin;
import com.elective.school.entity.Student;
import com.elective.school.entity.Teacher;
import com.elective.school.service.AdminService;
import com.elective.school.service.StudentService;
import com.elective.school.service.TeacherService;

@Controller
@SessionAttributes(value = { "admin", "teacher", "student" })
public class LoginController {

	@Autowired
	AdminService adminService;
	@Autowired
	StudentService studentService;
	@Autowired
	TeacherService teacherService;

	@RequestMapping("")
	public String login() {
		return "login";
	}

	@PostMapping("/login")
	public String login2(@RequestParam(name = "username", required = false) String username, HttpSession httpSession,
			@RequestParam(value = "password", required = false) String password,
			@RequestParam(value = "type", required = false) String type, Map<String, Object> map) {
		if (type.equals("admin")) {
			Admin admin = adminService.login(username, password);
			if (admin == null) {
				map.put("msg", "账号或密码错误！！！");
			} else {
				map.put("admin", admin);
				return "redirect:/admin";
			}
		}
		if (type.equals("student")) {
			Student student = studentService.login(username, password);
			if (student == null) {
				map.put("msg", "账号或密码错误！！！");
			} else {
				map.put("student", student);
				return "redirect:/student";
			}
		}
		if (type.equals("teacher")) {
			Teacher teacher = teacherService.login(username, password);
			if (teacher == null) {
				map.put("msg", "账号或密码错误！！！");
			} else {
				map.put("teacher", teacher);
				return "redirect:/teacher";
			}
		}

		return "login";
	}

	@GetMapping("/login")
	public String login1() {
		return "login";
	}

	@RequestMapping("/logout")
	public String logout(HttpSession session, SessionStatus sessionStatus) {
		session.invalidate();
		sessionStatus.setComplete();
		return "redirect:/login";
	}

}
