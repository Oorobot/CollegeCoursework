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

	@GetMapping("info")
	public String info(Map<String, Object> map) {
		map.putAll(adminService.getAcademies());
		map.put("operate", "info");
		map.put("adr", "student/info/save");
		return "student";
	}

	@PostMapping("/info/save")
	public String infoSave(Map<String, Object> map, @RequestParam(name = "student") String[] student,
			@RequestParam(name = "operate") String operate, HttpSession session) {
		Student s = (Student) session.getAttribute("student");
		map.putAll(adminService.validateStudent(student));
		map.putAll(adminService.getAcademies());
		map.put("operate", "info");
		map.put("adr", "student/info/save");
		if (map.get("error") != null) {
			return "student";
		}
		Student st = (Student) map.get("student");
		st.setPassword(s.getPassword());
		adminService.save(st);
		map.put("student", st);
		return "student";
	}

	@GetMapping("/select")
	public String select(Map<String, Object> map) {

		return "student-home";
	}
}
