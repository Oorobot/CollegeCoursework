package com.elective.school.service.Impl;

import java.sql.Date;
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
import com.elective.school.util.Method;

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
		if (admin != null && Method.md5Encoding(password).equals(admin.getPassword()))
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
		map.put("category", "term");
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
	public Map<String, Object> getAcademies() {
		Map<String, Object> map = new HashMap<String, Object>();
		map.put("academies", academyDao.findAll(Sort.by(new Order(Direction.ASC, "ano"))));
		map.put("category", "academy");
		return map;
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
		List<Course> courses = courseDao.findAll(Sort.by(new Order(Direction.ASC, "cno")));
		map.put("courses", courses);
		List<Academy> academies = academyDao.findAll();
		Map<String, Object> academyName = new HashMap<String, Object>();
		for (Academy a : academies) {
			academyName.put(a.getAno(), a.getName());
		}
		map.put("academyName", academyName);
		map.put("category", "course");
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
		List<Student> students = studentDao.findAll(Sort.by(new Order(Direction.ASC, "sno")));
		map.put("students", students);
		List<Academy> academies = academyDao.findAll();
		Map<String, Object> academyName = new HashMap<String, Object>();
		for (Academy a : academies) {
			academyName.put(a.getAno(), a.getName());
		}
		map.put("academyName", academyName);
		map.put("category", "student");
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
		List<Teacher> teachers = teacherDao.findAll(Sort.by(new Order(Direction.ASC, "tno")));
		map.put("teachers", teachers);
		List<Academy> academies = academyDao.findAll();
		Map<String, Object> academyName = new HashMap<String, Object>();
		for (Academy a : academies) {
			academyName.put(a.getAno(), a.getName());
		}
		map.put("academyName", academyName);
		map.put("category", "teacher");
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
	public Boolean exist(String term) {
		Example<Term> t = Example.of(new Term(null, term, null));
		return termDao.exists(t);
	}

	@Override
	public Map<String, Object> validateAcademy(Academy academy) {
		Map<String, Object> map = new HashMap<String, Object>();
		if (!academy.getAno().matches(Constant.reg_academy_ano)) {
			map.put("error", "学院号由两位数字组成");
		} else if (academy.getName() == null) {
			map.put("error", "学院名不为空");
		}
		return map;
	}

	@Override
	public Map<String, Object> exist(Academy academy) {
		// 先验证数据
		Map<String, Object> map = new HashMap<String, Object>();
		Example<Academy> ano = Example.of(new Academy(academy.getAno(), null, null, null));
		Example<Academy> name = Example.of(new Academy(null, academy.getName(), null, null));
		if (academyDao.exists(ano)) {
			map.put("error", "学院号已存在");
		} else if (academyDao.exists(name)) {
			map.put("error", "学院名已存在");
		} else {
			map.put("success", "操作成功");
		}
		return map;
	}

	@Override
	public Map<String, Object> validateCourse(String[] course) {
		Map<String, Object> map = new HashMap<String, Object>();
		if (course.length == 5) {
			if (!course[0].matches(Constant.reg_course_cno)) {
				map.put("error", "课程号由八位数字组成");
			} else if (course[1] == null) {
				map.put("error", "课程名不能为空");
			} else if (!course[2].matches(Constant.reg_num)) {
				map.put("error", "学分应为数字");
			} else if (!course[3].matches(Constant.reg_num)) {
				map.put("error", "学时应为数字");
			}
		}
		return map;
	}

	@Override
	public Map<String, Object> exist(Course course) {
		// 先验证数据
		Map<String, Object> map = new HashMap<String, Object>();
		Example<Course> cno = Example.of(new Course(course.getCno(), null, null, null, null));
		Example<Course> name = Example.of(new Course(null, course.getName(), null, null, null));
		if (courseDao.exists(cno)) {
			map.put("error", "课程号已存在");
		} else if (courseDao.exists(name)) {
			map.put("error", "课程名已存在");
		} else {
			map.put("success", "操作成功");
		}
		return map;
	}

	@Override
	public Map<String, Object> validateStudent(String[] student) {
		Map<String, Object> map = new HashMap<String, Object>();
		if (student.length == 7) {
			if (!student[0].matches(Constant.reg_student_sno)) {
				map.put("error", "学号为四位数字");
			} else if (student[1] == null) {
				map.put("error", "姓名不能为空");
			} else if (!student[3].matches(Constant.reg_date)) {
				map.put("error", "出生日期格式为：YYYY-MM-DD");
			} else if (student[4] == null) {
				map.put("error", "籍贯不能为空");
			} else if (student[5] == null) {
				map.put("error", "联系电话不能为空");
			}
		}
		if (map.size() == 0) {
			Student s = new Student();
			s.setSno(student[0]);
			s.setName(student[1]);
			s.setSex(Boolean.getBoolean(student[2]));
			s.setBirthday(Date.valueOf(student[3]));
			s.setPassword(Method.md5Encoding("123456"));
			s.setHometown(student[4]);
			s.setPhone(student[5]);
			s.setAno(student[6]);
			map.put("student", s);
		}
		return map;
	}

	@Override
	public Map<String, Object> exist(Student student) {
		Map<String, Object> map = new HashMap<String, Object>();
		Student temp = new Student();
		temp.setSno(student.getSno());
		Example<Student> sno = Example.of(temp);
		if (studentDao.exists(sno)) {
			map.put("error", "学号已存在");
		} else {
			map.put("success", "添加成功");
		}
		return map;
	}

	@Override
	public Map<String, Object> validateTeacher(String[] teacher) {
		Map<String, Object> map = new HashMap<String, Object>();
		if (teacher.length == 7) {
			if (!teacher[0].matches(Constant.reg_teacher_tno)) {
				map.put("error", "工号为4位数字");
			} else if (teacher[1] == null) {
				map.put("error", "姓名不能为空");
			} else if (!teacher[3].matches(Constant.reg_date)) {
				map.put("error", "出生日期格式为：YYYY-MM-DD");
			} else if (teacher[4] == null) {
				map.put("error", "职称不能为空");
			} else if (!teacher[5].matches(Constant.reg_double)) {
				map.put("error", "请输入整数或浮点数");
			}
		}
		if (map.size() == 0) {
			Teacher t = new Teacher(teacher[0], Method.md5Encoding("123456"), teacher[1], Boolean.valueOf(teacher[2]),
					Date.valueOf(teacher[3]), teacher[4], Double.valueOf(teacher[5]), teacher[6]);
			map.put("teacher", t);
		}
		return map;
	}

	@Override
	public Map<String, Object> exist(Teacher teacher) {
		Map<String, Object> map = new HashMap<String, Object>();
		if (teacherDao.existsById(teacher.getTno())) {
			map.put("error", "工号已存在");
		} else {
			map.put("success", "操作成功");
		}
		return map;
	}

}
