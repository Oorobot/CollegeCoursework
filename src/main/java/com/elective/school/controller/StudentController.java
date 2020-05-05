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

import com.elective.school.entity.Student;
import com.elective.school.service.Impl.AdminServiceImpl;

@Controller
@RequestMapping("/student")
@SessionAttributes(value = { "student" })
public class StudentController {

	@Autowired
	AdminServiceImpl adminService;

	@GetMapping("")
	public String login(Map<String, Object> map, HttpSession session) {
		Student student = (Student) session.getAttribute("student");
		if (student == null) {
			map.put("msg", "已过期，请重新登录");
			return "forward:/login";
		}
		map.put("student", student);
		return "student-home";
	}
}
