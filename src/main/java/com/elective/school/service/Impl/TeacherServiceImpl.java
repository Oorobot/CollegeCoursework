package com.elective.school.service.Impl;

import java.util.HashMap;
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
import com.elective.school.entity.Teacher;
import com.elective.school.service.TeacherService;
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

}
