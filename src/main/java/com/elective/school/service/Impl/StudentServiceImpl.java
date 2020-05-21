package com.elective.school.service.Impl;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Optional;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import com.elective.school.dao.AcademyDao;
import com.elective.school.dao.CourseDao;
import com.elective.school.dao.CourseScheduleDao;
import com.elective.school.dao.ElectiveDao;
import com.elective.school.dao.StudentDao;
import com.elective.school.dao.TeacherDao;
import com.elective.school.dao.TermDao;
import com.elective.school.entity.Course;
import com.elective.school.entity.CourseSchedule;
import com.elective.school.entity.CourseScheduleUPK;
import com.elective.school.entity.Elective;
import com.elective.school.entity.ElectiveUPK;
import com.elective.school.entity.Student;
import com.elective.school.entity.Term;
import com.elective.school.service.StudentService;
import com.elective.school.util.Method;

@Service("StudentService")
public class StudentServiceImpl implements StudentService {

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
	public Student login(String sno, String password) {
		Optional<Student> student = studentDao.findById(sno);
		if (student.isPresent() && Method.md5Encoding(password).equals(student.get().getPassword()))
			return student.get();
		return null;
	}

	@Override
	public Map<String, Object> getTermOnSelect() {
		Map<String, Object> map = new HashMap<String, Object>();
		List<Term> terms = termDao.findByStatus(1);
		map.put("terms", terms);
		return map;
	}

	@Override
	@Transactional(readOnly = true)
	public Map<String, Object> select(String termId, String cno, String cname, String tno, String tname) {
		Map<String, Object> map = new HashMap<String, Object>();
		List<CourseSchedule> cs = csDao.findByTermIdLikeTnoAndCno(Integer.valueOf(termId), tno, tname, cno, cname);
		map.put("CourseSchedules", cs);
		Map<String, String> tName = new HashMap<String, String>();
		Map<String, String> cName = new HashMap<String, String>();
		for (CourseSchedule c : cs) {
			tName.put(c.getUpk().getTno(), teacherDao.findById(c.getUpk().getTno()).get().getName());
			cName.put(c.getUpk().getCno(), courseDao.findById(c.getUpk().getCno()).get().getName());
		}
		map.put("teacherName", tName);
		map.put("courseName", cName);
		return map;
	}

	@Override
	@Transactional
	public Map<String, Object> saveElective(ElectiveUPK upk) {
		Map<String, Object> map = new HashMap<String, Object>();
		if (!electiveDao.existsById(upk)) {
			// 学生选课，课程人数+1
			CourseScheduleUPK csUPK = new CourseScheduleUPK(upk.getTermId(), upk.getCno(), upk.getTno());
			CourseSchedule cs = csDao.findById(csUPK).get();
			cs.setNumber(cs.getNumber() + 1);
			csDao.save(cs);
			Elective elective = new Elective(upk, 0, 0, 0);
			electiveDao.save(elective);
			map.put("success", "选课成功");
			return map;
		}
		map.put("error", "课程已选");
		return map;
	}

	@Override
	@Transactional
	public Map<String, Object> deleteElective(ElectiveUPK upk) {
		Map<String, Object> map = new HashMap<String, Object>();
		if (electiveDao.existsById(upk)) {
			// 学生退课，课程人数-1
			CourseScheduleUPK csUPK = new CourseScheduleUPK(upk.getTermId(), upk.getCno(), upk.getTno());
			CourseSchedule cs = csDao.findById(csUPK).get();
			cs.setNumber(cs.getNumber() - 1);
			csDao.save(cs);
			electiveDao.deleteById(upk);
			map.put("success", "退课成功");
			return map;
		}
		map.put("error", "该课程不存在");
		return map;
	}

	@Override
	public Map<String, Object> drop(String termId, String sno) {
		Map<String, Object> map = new HashMap<String, Object>();
		List<Elective> electives = electiveDao.findBySnoAndTermId(sno, Integer.valueOf(termId));
		List<CourseScheduleUPK> upks = new ArrayList<CourseScheduleUPK>();
		for (Elective e : electives) {
			CourseScheduleUPK upk = new CourseScheduleUPK(e.getUpk().getTermId(), e.getUpk().getCno(),
					e.getUpk().getTno());
			upks.add(upk);
		}
		List<CourseSchedule> cs = csDao.findAllById(upks);
		map.put("CourseSchedules", cs);
		Map<String, String> tName = new HashMap<String, String>();
		Map<String, String> cName = new HashMap<String, String>();
		for (CourseSchedule c : cs) {
			tName.put(c.getUpk().getTno(), teacherDao.findById(c.getUpk().getTno()).get().getName());
			cName.put(c.getUpk().getCno(), courseDao.findById(c.getUpk().getCno()).get().getName());
		}
		map.put("teacherName", tName);
		map.put("courseName", cName);
		return map;
	}

	@Override
	@Transactional
	public Map<String, Object> score(String termId, String sno) {
		Map<String, Object> map = new HashMap<String, Object>();
		List<Elective> electives = new ArrayList<Elective>();
		Integer id = Integer.valueOf(termId);
		Map<Integer, Integer> termScore = new HashMap<Integer, Integer>();// 每个学期的平均成绩
		if (id == -1) {
			electives = electiveDao.findBySno(sno);
			List<Integer> ids = new ArrayList<Integer>();
			for (Elective e : electives)
				ids.add(e.getUpk().getTermId());
			List<Term> terms = termDao.findAllById(ids);
			map.put("termsOfElectives", terms);
			for (Integer i : ids) {
				termScore.put(i, electiveDao.getAveragePerTerm(i, sno));
			}
			map.put("termScore", termScore);
			map.put("AlltermScore", electiveDao.getAverageAllTerm(sno));

		} else {
			electives = electiveDao.findBySnoAndTermId(sno, id);
			Integer score = electiveDao.getAveragePerTerm(id, sno);
			if (score != null)
				map.put("termScore", score);
		}
		Map<String, String> cName = new HashMap<String, String>();
		Map<String, Integer> cCredit = new HashMap<String, Integer>();
		for (Elective e : electives) {
			Course c_temp = courseDao.findById(e.getUpk().getCno()).get();
			cName.put(e.getUpk().getCno(), c_temp.getName());
			cCredit.put(e.getUpk().getCno(), c_temp.getCredit());
		}
		map.put("courseName", cName);
		map.put("courseCredit", cCredit);
		map.put("electives", electives);
		return map;
	}
}
