package com.elective.school.dao;

import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.Query;

import com.elective.school.entity.Teacher;

public interface TeacherDao extends JpaRepository<Teacher, String> {

	@Query(value = "select a.name from Academy a where a.ano in (select t.ano from Teacher t where t.tno = ?1)")
	public String findAcademyNameByTno(String tno);

	@Query(value = "Select t from Teacher t where t.name = ?1")
	public Teacher findByName(String name);

}
