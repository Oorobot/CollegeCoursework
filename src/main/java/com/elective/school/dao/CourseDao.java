package com.elective.school.dao;

import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.Query;

import com.elective.school.entity.Course;

public interface CourseDao extends JpaRepository<Course, String> {
	@Query(value = "select a.name from Academy a where ano in"
			+ "(select c.ano from Course c where c.cno = ?1)")
	public String findAcademyNameByCno(String cno);
}
