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

import com.elective.school.entity.CourseSchedule;
import com.elective.school.entity.CourseScheduleUPK;
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
		map.put("adr", "teacher/info/save");
		return "teacher";
	}

	@PostMapping("/info/save")
	public String updateInfo(Map<String, Object> map, @RequestParam(name = "teacher") String[] teacher,
			HttpSession session) {
		Teacher st = (Teacher) session.getAttribute("teacher");
		map.putAll(adminService.validateTeacher(teacher));
		if (map.get("error") != null) {
			map.putAll(adminService.getAcademies());
			map.put("operate", "info");
			map.put("adr", "teacher/info/save");
			return "teacher";
		}
		Teacher t = (Teacher) map.get("teacher");
		t.setPassword(st.getPassword());
		adminService.save(t);
		map.put("teacher", t);
		map.putAll(adminService.getAcademies());
		map.put("operate", "info");
		map.put("adr", "teacher/info/save");
		return "teacher";
	}

	@GetMapping("/open")
	public String course(Map<String, Object> map) {
		map.putAll(teacherService.getCourses());
		return "teacher-home";
	}

	@GetMapping("/open/{cno}")
	public String courseOpen(Map<String, Object> map, @PathVariable(name = "cno") String cno) {
		map.put("course", adminService.getCourse(cno));
		map.put("terms", teacherService.getTermByStatus(0));
		map.put("operate", "add");
		map.put("adr", "open");
		return "teacher-cs";
	}

	@PostMapping("/open")
	public String csSave(Map<String, Object> map, @RequestParam(name = "cs") String[] cs,
			@RequestParam(name = "cno") String cno, HttpSession session) {
		Teacher teacher = (Teacher) session.getAttribute("teacher");
		map.putAll(teacherService.validateCS(cs));
		if (map.get("error") == null) {
			CourseScheduleUPK upk = new CourseScheduleUPK(Integer.valueOf(cs[0]), cno, teacher.getTno());
			map.putAll(teacherService.existCS(upk));
			if (map.get("error") == null) {
				CourseSchedule courseSchedule = new CourseSchedule();
				courseSchedule.setUpk(upk);
				courseSchedule.setTime(cs[1]);
				courseSchedule.setClassroom(cs[2]);
				courseSchedule.setNum(Integer.valueOf(cs[3]));
				courseSchedule.setWeight(Integer.valueOf(cs[4]));
				adminService.save(courseSchedule);
			}
		}
		map.putAll(teacherService.getCourses());
		return "teacher-home";
	}

	@GetMapping("/opened")
	public String courseOpened(Map<String, Object> map, @RequestParam(name = "termId", required = false) String termId,
			HttpSession session) {
		Teacher teacher = (Teacher) session.getAttribute("teacher");
		map.putAll(adminService.getTerms());
		if (termId != null) {
			map.put("term", adminService.getTerm(Integer.valueOf(termId)));
			map.putAll(teacherService.getCS(Integer.valueOf(termId), teacher.getTno()));
		}
		map.put("category", "opened");
		return "teacher-home";
	}

	@GetMapping("/opened/update/{termId}/{tno}/{cno}")
	public String courseOpenedUpdate(@PathVariable(name = "termId") String termId,
			@PathVariable(name = "tno") String tno, @PathVariable(name = "cno") String cno, Map<String, Object> map) {
		CourseScheduleUPK upk = new CourseScheduleUPK(Integer.valueOf(termId), cno, tno);
		CourseSchedule cs = adminService.getCS(upk);
		map.put("CourseSchedule", cs);
		map.put("course", adminService.getCourse(cs.getUpk().getCno()));
		map.put("term", adminService.getTerm(Integer.valueOf(termId)));
		map.put("operate", "update");
		map.put("adr", "opened");
		return "teacher-cs";
	}

	@PostMapping("/opened")
	public String courseOpenedUpdateSave(Map<String, Object> map, @RequestParam(name = "cs") String[] cs,
			@RequestParam(name = "cno") String cno, HttpSession session) {
		Teacher teacher = (Teacher) session.getAttribute("teacher");
		CourseScheduleUPK upk = new CourseScheduleUPK(Integer.valueOf(cs[0]), cno, teacher.getTno());
		CourseSchedule courseSchedule = adminService.getCS(upk);
		map.putAll(teacherService.validateCS(cs));
		if (map.get("error") == null) {
			courseSchedule.setTime(cs[1]);
			courseSchedule.setClassroom(cs[2]);
			courseSchedule.setNum(Integer.valueOf(cs[3]));
			courseSchedule.setWeight(Integer.valueOf(cs[4]));
			adminService.save(courseSchedule);
			String url = "redirect:/teacher/opened?termId=" + cs[0];
			return url;
		}

		
		map.put("CourseSchedule", courseSchedule);
		map.put("course", adminService.getCourse(courseSchedule.getUpk().getCno()));
		map.put("term", adminService.getTerm(upk.getTermId()));
		map.put("operate", "update");
		map.put("adr", "opened");
		return "teacher-cs";
	}
	@GetMapping("/opened/look/{termId}/{tno}/{cno}")
	public String courseOpenedStudent(@PathVariable(name = "termId") String termId, @PathVariable(name = "tno") String tno,
			@PathVariable(name = "cno") String cno, Map<String, Object> map) {
		CourseScheduleUPK upk = new CourseScheduleUPK(Integer.valueOf(termId), cno, tno);
		map.putAll(adminService.getElective(upk));
		return "teacher-home";
	}
}
