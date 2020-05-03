package com.elective.school.dao;

import java.util.List;

import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.Query;

import com.elective.school.entity.Elective;
import com.elective.school.entity.ElectiveUPK;

public interface ElectiveDao extends JpaRepository<Elective, ElectiveUPK>{

	@Query(value = "Select e from Elective e where e.upk.sno = ?1")
	public List<Elective> findBySno(String Sno);

	@Query(value = "Select e from Elective e where e.upk.cno = ?1")
	public List<Elective> findByCno(String Cno);

	@Query(value = "Select e from Elective e where e.upk.tno = ?1")
	public List<Elective> findByTno(String Tno);

	@Query(value = "Select e from Elective e where e.upk.termId = ?1")
	public List<Elective> findByTermId(Integer termId);
}
