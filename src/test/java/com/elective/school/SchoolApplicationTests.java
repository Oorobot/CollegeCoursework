package com.elective.school;

import org.junit.jupiter.api.Test;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.data.domain.Example;

import com.elective.school.dao.AcademyDao;
import com.elective.school.dao.AdminDao;
import com.elective.school.dao.CourseScheduleDao;
import com.elective.school.dao.StudentDao;
import com.elective.school.dao.TeacherDao;
import com.elective.school.dao.TermDao;
import com.elective.school.entity.Admin;
import com.elective.school.entity.Term;
import com.elective.school.util.Method;

@SpringBootTest
class SchoolApplicationTests {

	@Autowired
	AdminDao adminDao;
	@Autowired
	AcademyDao academyDao;
	@Autowired
	CourseScheduleDao courseScheduleDao;
	@Autowired
	StudentDao studentDao;
	@Autowired
	TermDao termDao;
	@Autowired
	TeacherDao teacherDao;

	@Test
	void contextLoads() {
		Admin admin = new Admin();
		admin.setId(1);
		admin.setName("admin");
		admin.setPassword(Method.md5Encoding("123456"));
		adminDao.save(admin);
		System.out.println(adminDao.findAll());

	}

	@Test
	void testexist() {
		Term t1 = new Term(null, "2015-2016春季", null);
		Example<Term> t = Example.of(t1);
		System.out.println(termDao.exists(t));
	}

	@Test
	void testDouble() {
		System.out.println(Double.valueOf("12131.123"));
		if (Double.valueOf("dasdsaf") == null) {
			System.out.println(11111);
		}
		System.out.println(Double.valueOf("dasdsafasd1321das1.22") == null);
		System.out.println(Double.valueOf("你餓了吧"));
	}

	@Test
	void testTeacherDao() {
		System.out.print(teacherDao.findByName("马小红"));
	}
}
