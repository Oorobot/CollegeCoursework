package com.elective.school.dao;

import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.Query;

import com.elective.school.entity.Admin;


public interface AdminDao extends JpaRepository<Admin, Integer> {
	
	@Query(value = "Select a from Admin a Where a.name = ?1")
	public Admin findByName(String name);
}
