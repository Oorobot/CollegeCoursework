package com.elective.school.service.Impl;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.data.domain.Example;
import org.springframework.data.domain.Sort;
import org.springframework.data.domain.Sort.Direction;
import org.springframework.data.domain.Sort.Order;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import com.elective.school.dao.AcademyDao;
import com.elective.school.dao.AdminDao;
import com.elective.school.dao.CourseDao;
import com.elective.school.dao.CourseScheduleDao;
import com.elective.school.dao.StudentDao;
import com.elective.school.dao.TeacherDao;
import com.elective.school.dao.TermDao;
import com.elective.school.entity.Academy;
import com.elective.school.entity.Admin;
import com.elective.school.entity.Course;
import com.elective.school.entity.CourseSchedule;
import com.elective.school.entity.Student;
import com.elective.school.entity.Teacher;
import com.elective.school.entity.Term;
import com.elective.school.service.AdminService;
import com.elective.school.util.Constant;
import com.elective.school.util.MD5;

@Service("AdminService")
public class AdminServiceImpl implements AdminService {

	@Autowired
	AdminDao adminDao;
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

	@Override
	public boolean login(String name, String password) {
		Admin admin = adminDao.findByName(name);
		if (admin != null && MD5.md5Encoding(password).equals(admin.getPassword()))
			return true;
		return false;
	}

	@Transactional(readOnly = true)
	@Override
	public Map<String, Object> getTerms() {
		Map<String, Object> map = new HashMap<String, Object>();
		List<Term> terms = termDao.findAll(Sort.by(new Order(Direction.ASC, "term")));
		map.put("terms", terms);
		map.put("term_status", Constant.term_status);
		return map;
	}

	@Override
	public Term getTerm(Integer termId) {
		if (termId == null)
			return new Term();
		else
			return termDao.findById(termId).get();

	}

	@Transactional
	@Override
	public void save(Term term) {
		termDao.save(term);
	}

	@Transactional(readOnly = true)
	@Override
	public List<Academy> getAcademies() {
		return academyDao.findAll();
	}

	@Transactional
	@Override
	public void save(Academy academy) {
		academyDao.save(academy);
	}

	@Transactional(readOnly = true)
	@Override
	public Map<String, Object> getCourses() {
		Map<String, Object> map = new HashMap<String, Object>();
		List<Course> courses = courseDao.findAll();
		Map<String, Object> academyName = new HashMap<String, Object>();
		for (Course c : courses) {
			academyName.put(c.getCno(), courseDao.findAcademyNameByCno(c.getCno()));
		}
		map.put("courses", courses);
		map.put("aName", academyName);
		return map;
	}

	@Transactional
	@Override
	public void save(Course course) {
		courseDao.save(course);
	}

	@Transactional
	@Override
	public void delete(Course course) {
		courseDao.delete(course);
	}

	@Transactional(readOnly = true)
	@Override
	public Map<String, Object> getStudents() {
		Map<String, Object> map = new HashMap<String, Object>();
		List<Student> students = studentDao.findAll();
		map.put("students", students);
		Map<String, Object> academyName = new HashMap<String, Object>();
		for (Student s : students) {
			academyName.put(s.getSno(), studentDao.findAcademyNameBySno(s.getSno()));
		}
		map.put("aName", academyName);
		return map;
	}

	@Override
	public void save(Student student) {
		studentDao.save(student);
	}

	@Override
	public void saveStudents(List<Student> students) {
		studentDao.saveAll(students);
	}

	@Override
	public void delete(Student student) {
		studentDao.delete(student);
	}

	@Override
	public Map<String, Object> getTeachers() {
		Map<String, Object> map = new HashMap<String, Object>();
		List<Teacher> teachers = teacherDao.findAll();
		map.put("teachers", teachers);
		Map<String, Object> academyName = new HashMap<String, Object>();
		for (Teacher t : teachers) {
			academyName.put(t.getTno(), teacherDao.findAcademyNameByTno(t.getTno()));
		}
		map.put("aName", academyName);
		return map;
	}

	@Override
	public void save(Teacher teacher) {
		teacherDao.save(teacher);
	}

	@Override
	public void saveTeachers(List<Teacher> teachers) {
		teacherDao.saveAll(teachers);
	}

	@Override
	public void delete(Teacher Teacher) {
		teacherDao.delete(Teacher);
	}

	@Override
	public Map<String, Object> getCS(Integer termId) {
		Map<String, Object> map = new HashMap<String, Object>();
		List<CourseSchedule> cs = csDao.findByTermId(termId);
		Map<String, String> tName = new HashMap<String, String>();
		Map<String, String> cName = new HashMap<String, String>();
		for (CourseSchedule c : cs) {
			tName.put(c.getUpk().getTno(), teacherDao.findById(c.getUpk().getTno()).get().getName());
			cName.put(c.getUpk().getCno(), courseDao.findById(c.getUpk().getTno()).get().getName());
		}
		map.put("CourseSchedules", cs);
		map.put("tName", tName);
		map.put("cName", cName);
		return map;
	}

	@Override
	public Academy getAcademy(String ano) {
		if (ano != null)
			return academyDao.findById(ano).get();
		return new Academy();
	}

	@Override
	public Course getCourse(String cno) {
		if (cno != null)
			return courseDao.findById(cno).get();
		return new Course();
	}

	@Override
	public Student getStudent(String sno) {
		if (sno != null)
			return studentDao.findById(sno).get();
		return new Student();

	}

	@Override
	public Teacher getTeacher(String tno) {
		if (tno != null)
			return teacherDao.findById(tno).get();
		else
			return new Teacher();
	}

	@Override
	public void save(CourseSchedule cs) {
		csDao.save(cs);
	}

	@Override
	public Boolean existTerm(String term) {
		Example<Term> t = Example.of(new Term(null, term, null));
		return termDao.exists(t);
	}

}
