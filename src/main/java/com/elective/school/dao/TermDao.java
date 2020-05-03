package com.elective.school.dao;

import org.springframework.data.jpa.repository.JpaRepository;
import com.elective.school.entity.Term;

public interface TermDao extends JpaRepository<Term, Integer> {

}
