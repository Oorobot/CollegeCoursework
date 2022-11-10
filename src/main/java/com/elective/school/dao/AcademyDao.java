package com.elective.school.dao;

import org.springframework.data.jpa.repository.JpaRepository;

import com.elective.school.entity.Academy;


public interface AcademyDao extends JpaRepository<Academy, String>{

}
