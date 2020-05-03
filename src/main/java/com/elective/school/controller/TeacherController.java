package com.elective.school.controller;

import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.PostMapping;

@Controller
public class TeacherController {

	@PostMapping("/teacher")
	public String login() {
		return "teacher";
	}
}
