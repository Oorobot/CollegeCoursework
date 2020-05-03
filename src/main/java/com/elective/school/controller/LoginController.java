package com.elective.school.controller;

import java.text.DateFormat;
import java.util.Date;

import javax.servlet.http.HttpSession;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.RequestMapping;

import com.elective.school.dao.AcademyDao;
import com.elective.school.dao.ElectiveDao;
import com.elective.school.dao.StudentDao;
import com.elective.school.entity.Student;


@Controller
public class LoginController {

	@Autowired
	AcademyDao academyDao;
	@Autowired
	ElectiveDao electiveDao;
	@Autowired
	StudentDao studentDao;

	@RequestMapping("")
	public String login(HttpSession session) {
		session.invalidate();
		return "login";
	}	
	
	
	@RequestMapping("/test")
	public String test(Model m) {
		
		m.addAttribute("now", DateFormat.getDateInstance().format(new Date()));
		m.addAttribute("Student", new Student());
		System.out.print(11111);
		return "test";
	}
	
	
	@RequestMapping("/addtest")
	public String addtest(Model m) {
		
		return "addtest";
	}
	
}
