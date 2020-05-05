package com.elective.school.dao;

import java.util.List;

import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.Query;

import com.elective.school.entity.Student;

public interface StudentDao extends JpaRepository<Student, String> {

	@Query(value = "Select s from Student s Where s.sno Like %?1%")
	public List<Student> findLikeSno(String sno);

	@Query(value = "Select a.name from Academy a Where ano in (select s.ano from Student s where s.sno = ?1)")
	public String findAcademyNameBySno(String sno);

	@Query(value = "Select s from Student s where s.name = ?1")
	public Student findByName(String name);
}
