package com.elective.school.controller;

import java.util.Map;

import javax.servlet.http.HttpSession;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.SessionAttributes;

import com.elective.school.entity.ElectiveUPK;
import com.elective.school.entity.Student;
import com.elective.school.entity.Term;
import com.elective.school.service.AdminService;
import com.elective.school.service.StudentService;
import com.elective.school.util.Method;

@Controller
@RequestMapping("/student")
@SessionAttributes(value = { "student" })
public class StudentController {

	@Autowired
	AdminService adminService;
	@Autowired
	StudentService studentService;

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
	public String select(Map<String, Object> map, @RequestParam(name = "termId", required = false) String termId,
			@RequestParam(name = "cno", required = false) String cno,
			@RequestParam(name = "cname", required = false) String cname,
			@RequestParam(name = "tno", required = false) String tno,
			@RequestParam(name = "tname", required = false) String tname) {
		map.putAll(studentService.getTermOnSelect());
		if (termId != null && tname != null && cno != null && tno != null && cname != null) {
			map.put("term", adminService.getTerm(Integer.valueOf(termId)));
			map.putAll(studentService.select(termId, cno, cname, tno, tname));
			map.put("cno", cno);
			map.put("cname", cname);
			map.put("tno", tno);
			map.put("tname", tname);
		}
		return "student-select";
	}

	@GetMapping("/select/{termId}/{tno}/{cno}")
	public String selectCourse(Map<String, Object> map, HttpSession session,
			@PathVariable(name = "termId") String termId, @PathVariable(name = "tno") String tno,
			@PathVariable(name = "cno") String cno, @RequestParam(name = "search") String[] search) {
		Student student = (Student) session.getAttribute("student");
		ElectiveUPK upk = new ElectiveUPK(student.getSno(), Integer.valueOf(termId), cno, tno);
		map.putAll(studentService.saveElective(upk));
		String forward = "forward:/student/select?termId=" + termId + "&cno=" + search[0] + "&cname=" + search[1]
				+ "&tno=" + search[2] + "&tname=" + search[3];
		return forward;
	}

	@GetMapping("/drop")
	public String drop(HttpSession session, Map<String, Object> map,
			@RequestParam(name = "termId", required = false) String termId) {
		map.putAll(adminService.getTerms());
		Student student = (Student) session.getAttribute("student");
		if (termId != null) {
			map.put("term", adminService.getTerm(Integer.valueOf(termId)));
			map.putAll(studentService.drop(termId, student.getSno()));
		}
		return "student-drop";
	}

	@GetMapping("/drop/{termId}/{tno}/{cno}")
	public String dropCourse(HttpSession session, Map<String, Object> map, @PathVariable(name = "termId") String termId,
			@PathVariable(name = "tno") String tno, @PathVariable(name = "cno") String cno) {
		Student student = (Student) session.getAttribute("student");
		ElectiveUPK upk = new ElectiveUPK(student.getSno(), Integer.valueOf(termId), cno, tno);
		map.putAll(studentService.deleteElective(upk));
		return "forward:/student/drop?termId=" + termId;
	}

	@GetMapping("/score")
	public String score(HttpSession session, Map<String, Object> map,
			@RequestParam(name = "termId", required = false) String termId) {
		Student student = (Student) session.getAttribute("student");
		map.putAll(adminService.getTerms());
		if (termId != null) {
			if (Integer.valueOf(termId) == -1) {
				map.put("term", new Term(-1, null, null));
			} else {
				map.put("term", adminService.getTerm(Integer.valueOf(termId)));
			}
			map.putAll(studentService.score(termId, student.getSno()));
		}
		return "student-score";
	}
	
	@GetMapping("/password")
	public String password(Map<String, Object> map) {
		map.put("adr", "student/password");
		return "password";
	}
	@PostMapping("/password")
	public String updatePassword(Map<String, Object> map, @RequestParam(name = "password1") String password1,
			@RequestParam(name = "password2") String password2, HttpSession session) {
		Student student = (Student) session.getAttribute("student");
		map.put("adr", "student/password");
		if (password1.equals(password2)) {
			student.setPassword(Method.md5Encoding(password1));
			adminService.save(student);
			map.put("teacher", student);
			map.put("success", "修改成功");
		} else {
			map.put("error", "两次输入密码不正确");
		}
		return "password";
	}
}
