package com.elective.school.service.Impl;

import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Optional;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.data.domain.Example;
import org.springframework.data.domain.Sort;
import org.springframework.data.domain.Sort.Direction;
import org.springframework.data.domain.Sort.Order;
import org.springframework.stereotype.Service;

import com.elective.school.dao.AcademyDao;
import com.elective.school.dao.CourseDao;
import com.elective.school.dao.CourseScheduleDao;
import com.elective.school.dao.ElectiveDao;
import com.elective.school.dao.StudentDao;
import com.elective.school.dao.TeacherDao;
import com.elective.school.dao.TermDao;
import com.elective.school.entity.Academy;
import com.elective.school.entity.CourseSchedule;
import com.elective.school.entity.CourseScheduleUPK;
import com.elective.school.entity.Teacher;
import com.elective.school.entity.Term;
import com.elective.school.service.TeacherService;
import com.elective.school.util.Constant;
import com.elective.school.util.Method;

@Service("TeacherService")
public class TeacherServiceImpl implements TeacherService {

	@Autowired
	TermDao termDao;
	@Autowired
	AcademyDao academyDao;
	@Autowired
	CourseDao courseDao;
	@Autowired
	TeacherDao teacherDao;
	@Autowired
	StudentDao studentDao;
	@Autowired
	CourseScheduleDao csDao;
	@Autowired
	ElectiveDao electiveDao;

	@Override
	public Teacher login(String tno, String password) {
		Optional<Teacher> teacher = teacherDao.findById(tno);
		if (teacher.isPresent() && Method.md5Encoding(password).equals(teacher.get().getPassword())) {
			return teacher.get();
		}
		return null;
	}

	@Override
	public Map<String, Object> getTeacherInfo(String tno) {
		Map<String, Object> map = new HashMap<String, Object>();
		map.put("teacher", teacherDao.findById(tno).get());
		map.put("academyName", teacherDao.findAcademyNameByTno(tno));
		return map;
	}

	@Override
	public Map<String, Object> getCourses() {
		Map<String, Object> map = new HashMap<String, Object>();
		map.put("courses", courseDao.findAll(Sort.by(new Order(Direction.ASC, "cno"))));
		map.put("category", "open");
		map.put("terms", termDao.findByStatus(0));
		List<Academy> academies = academyDao.findAll();
		Map<String, Object> academyName = new HashMap<String, Object>();
		for (Academy a : academies) {
			academyName.put(a.getAno(), a.getName());
		}
		map.put("academyName", academyName);
		return map;
	}

	@Override
	public List<Term> getTermByStatus(Integer status) {
		return termDao.findByStatus(status);
	}

	@Override
	public Map<String, Object> validateCS(String[] cs) {
		Map<String, Object> map = new HashMap<String, Object>();
		if (cs.length == 5) {
			if (cs[1].isEmpty()) {
				map.put("error", "上课时间不能为空");
			} else if (cs[2].isEmpty()) {
				map.put("error", "上课地点不能为空");
			} else if (!cs[3].matches(Constant.reg_num)) {
				map.put("error", "请填写数字");
			} else if (!cs[4].matches(Constant.reg_num)) {
				map.put("error", "请填写数字");
			} else if (cs[4].matches(Constant.reg_num)) {
				if (Integer.valueOf(cs[4]) < 0 || Integer.valueOf(cs[4]) > 100)
					map.put("error", "请填写0~100之间的整数");
			}
		}
		return map;
	}

	@Override
	public Map<String, Object> existCS(CourseScheduleUPK upk) {
		CourseSchedule example = new CourseSchedule();
		example.setUpk(upk);
		Example<CourseSchedule> cs = Example.of(example);
		Map<String, Object> map = new HashMap<String, Object>();
		if (csDao.exists(cs)) {
			map.put("error", "课程已开设");
		} else {
			map.put("success", "成功开设课程");
		}
		return map;
	}

	@Override
	public Map<String, Object> getCS(Integer termId, String tno) {
		Map<String, Object> map = new HashMap<String, Object>();
		List<CourseSchedule> cs = csDao.findByTermIdAndTno(termId, tno);
		Map<String, String> tName = new HashMap<String, String>();
		Map<String, String> cName = new HashMap<String, String>();
		for (CourseSchedule c : cs) {
			tName.put(c.getUpk().getTno(), teacherDao.findById(c.getUpk().getTno()).get().getName());
			cName.put(c.getUpk().getCno(), courseDao.findById(c.getUpk().getCno()).get().getName());
		}
		map.put("CourseSchedules", cs);
		map.put("teacherName", tName);
		map.put("courseName", cName);
		map.put("category", "opened");
		return map;
	}

}
