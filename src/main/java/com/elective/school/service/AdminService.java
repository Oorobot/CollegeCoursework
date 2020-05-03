package com.elective.school.service;

import java.util.List;
import java.util.Map;

import com.elective.school.entity.Academy;
import com.elective.school.entity.Course;
import com.elective.school.entity.CourseSchedule;
import com.elective.school.entity.Student;
import com.elective.school.entity.Teacher;
import com.elective.school.entity.Term;

public interface AdminService {

	//学期管理
	public boolean login(String name, String password);
	public Map<String, Object> getTerms();
	public Term getTerm(Integer termId);
	public void save(Term term);
	public Boolean existTerm(String term);
	//学院管理
	public List<Academy> getAcademies();
	public Academy getAcademy(String ano);
	public void save(Academy academy);
	//课程管理
	public Map<String, Object> getCourses();
	public Course getCourse(String cno);
	public void save(Course course);
	public void delete(Course course);
	//学生管理
	public Map<String, Object> getStudents();
	public Student getStudent(String sno);
	public void save(Student student);
	public void saveStudents(List<Student> students);
	public void delete(Student student);
	//教师管理
	public Map<String,Object> getTeachers();
	public Teacher getTeacher(String tno);
	public void save(Teacher teacher);
	public void saveTeachers(List<Teacher> teachers);
	public void delete(Teacher Teacher);
	//开课信息
	public Map<String,Object> getCS(Integer termId);
	public void save(CourseSchedule cs);
}
