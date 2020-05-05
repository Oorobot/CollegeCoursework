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

import com.elective.school.entity.Teacher;
import com.elective.school.service.AdminService;
import com.elective.school.service.TeacherService;

@Controller
@RequestMapping("/teacher")
@SessionAttributes(value = { "teacher" })
public class TeacherController {

	@Autowired
	AdminService adminService;
	@Autowired
	TeacherService teacherService;

	@GetMapping("")
	public String home(Map<String, Object> map, HttpSession session) {
		Teacher teacher = (Teacher) session.getAttribute("teacher");
		if (teacher == null) {
			map.put("msg", "已过期，请重新登录");
			return "forward:/login";
		}
		map.putAll(teacherService.getTeacherInfo(teacher.getTno()));
		return "teacher-home";
	}

	@GetMapping("/info")
	public String info(Map<String, Object> map) {
		map.putAll(adminService.getAcademies());
		map.put("operate", "info");
		map.put("adr", "teacher");
		return "teacher";
	}

	@PostMapping("")
	public String updateInfo(Map<String, Object> map, @RequestParam(name = "teacher") String[] teacher) {
		map.putAll(adminService.validateTeacher(teacher));
		if (map.get("error") != null) {
			map.putAll(adminService.getAcademies());
			map.put("operate", "info");
			map.put("adr", "teacher");
			return "teacher";
		}
		Teacher t = (Teacher) map.get("teacher");
		adminService.save(t);
		return "teacher-home";
	}
}
