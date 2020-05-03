package com.elective.school;

import java.util.List;

import org.junit.jupiter.api.Test;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.data.domain.Example;
import org.springframework.validation.annotation.Validated;

import com.elective.school.dao.AcademyDao;
import com.elective.school.dao.AdminDao;
import com.elective.school.dao.CourseScheduleDao;
import com.elective.school.dao.StudentDao;
import com.elective.school.dao.TermDao;
import com.elective.school.entity.Academy;
import com.elective.school.entity.Admin;
import com.elective.school.entity.Student;
import com.elective.school.entity.Term;
import com.elective.school.util.MD5;

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

	@Test
	void contextLoads() {
//		Academy a = new Academy("110", "001", "002", "003");
//		System.out.println(a);
//		academyDao.save(a);
//		a.setName("9971");
//		System.out.println(a);
//		academyDao.save(a);
//		System.out.println(courseScheduleDao.findByCno("1101"));
//		System.out.println(courseScheduleDao.findByTno("2202"));
//		System.out.println(courseScheduleDao.findByTermId(1));
//		String pw = MD5.md5Encoding("123456");
//		System.out.println("MD5加密后:"+pw+",长度为："+pw.length());
//		pw = MD5.md5Encoding("dasf1546");
//		System.out.println("MD5加密后:"+pw+",长度为："+pw.length());
		Admin admin = new Admin();
		admin.setId(1);
		admin.setName("admin");
		admin.setPassword(MD5.md5Encoding("123456"));
		adminDao.save(admin);
		System.out.println(adminDao.findAll());
		
//		Student stu = new Student();
//		stu.setSno("1103");
//		stu.setAno("02");
//		studentDao.save(stu);
//		List<Student> s = studentDao.findAll();
//		if(!s.isEmpty()) {
//			System.out.println(s);
//		}
//		System.out.println("kong");
//		System.out.println(studentDao.findAll());
//		System.out.println(studentDao.findLikeSno("1101"));

	}
	@Test
	void testexist(){
		Term t1=new Term(null, "2015-2016春季", null);
		Example<Term> t=Example.of(t1);
		System.out.println(termDao.exists(t));
	}
	


}
