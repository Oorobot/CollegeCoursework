package com.elective.school.service.Impl;

import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Optional;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.elective.school.dao.AcademyDao;
import com.elective.school.dao.CourseDao;
import com.elective.school.dao.CourseScheduleDao;
import com.elective.school.dao.ElectiveDao;
import com.elective.school.dao.StudentDao;
import com.elective.school.dao.TeacherDao;
import com.elective.school.dao.TermDao;
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
	public Map<String, Object> select() {
		Map<String,Object> map = new HashMap<String, Object>();
		List<Term> terms = termDao.findByStatus(1);
		map.put("terms", terms);
		return map;
	}

}
