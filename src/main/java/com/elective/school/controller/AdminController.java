package com.elective.school.controller;

import java.util.Map;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.validation.annotation.Validated;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestParam;

import com.elective.school.entity.Term;
import com.elective.school.service.Impl.AdminServiceImpl;
import com.elective.school.util.Constant;

@Validated
@Controller
public class AdminController {

	@Autowired
	AdminServiceImpl adminService;

	@PostMapping("/admin")
	public String login(@RequestParam(name = "username", required = false) String username,
			@RequestParam(value = "password", required = false) String password, Map<String, Object> map) {
		if (adminService.login(username, password))
			return "admin";
		map.put("msg", "账号或密码错误！！！");
		return "forward:";
	}

	@GetMapping("/admin/term")
	public String term(Map<String, Object> map) {
		map.putAll(adminService.getTerms());
		map.put("category", "term");
		return "admin";
	}

	@PostMapping("admin/term")
	public String termAdd(Map<String, Object> map, @RequestParam(name = "termName") String termName) {
		if (adminService.existTerm(termName)) {
			map.put("addTermError", "该学期已存在，请重新添加");
			map.putAll(adminService.getTerms());
			map.put("category", "term");
			return "admin";
		}
		if (termName.matches(Constant.reg_term_status)) {
			Term t = new Term(null, termName, 0);
			adminService.save(t);
			map.put("addTermSuccess", "添加成功");
			map.putAll(adminService.getTerms());
			map.put("category", "term");
			return "admin";
		}
		map.put("addTermError", "输入格式有误，请重新添加");
		map.putAll(adminService.getTerms());
		map.put("category", "term");
		return "admin";

	}

	@RequestMapping("/admin/term/{id}/{status}")
	public String termUpdate(@PathVariable(name = "id") Integer id, @PathVariable(name = "status") Integer status) {
		System.out.println(id + "   " + status);
		Term t = adminService.getTerm(id);
		t.setStatus(status);
		adminService.save(t);
		return "redirect:/admin/term";
	}
}
