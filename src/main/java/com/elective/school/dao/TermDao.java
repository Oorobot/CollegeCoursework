package com.elective.school.dao;

import java.util.List;

import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.Query;

import com.elective.school.entity.Term;

public interface TermDao extends JpaRepository<Term, Integer> {

	@Query(value = "Select t from Term t Where t.status = ?1")
	List<Term> findByStatus(Integer Status);

}
