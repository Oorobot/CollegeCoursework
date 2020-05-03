package com.elective.school.controller;

import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.PostMapping;

@Controller
public class StudentController {

	@PostMapping("student")
	public String login() {
		return "student";
	}
}
