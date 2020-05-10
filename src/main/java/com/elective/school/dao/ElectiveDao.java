package com.elective.school.dao;

import java.util.List;

import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.Query;

import com.elective.school.entity.Elective;
import com.elective.school.entity.ElectiveUPK;

public interface ElectiveDao extends JpaRepository<Elective, ElectiveUPK> {

	@Query(value = "Select e from Elective e where e.upk.sno = ?1")
	public List<Elective> findBySno(String sno);

	@Query(value = "Select e from Elective e where e.upk.cno = ?1")
	public List<Elective> findByCno(String cno);

	@Query(value = "Select e from Elective e where e.upk.tno = ?1")
	public List<Elective> findByTno(String tno);

	@Query(value = "Select e from Elective e where e.upk.termId = ?1")
	public List<Elective> findByTermId(Integer termId);

	@Query(value = "Select e from Elective e where e.upk.sno = ?1 and e.upk.termId = ?2")
	public List<Elective> findBySnoAndTermId(String sno, Integer termId);

	@Query(value = "Select e from Elective e where e.upk.termId = ?1 and e.upk.cno = ?2 and e.upk.tno = ?3")
	public List<Elective> findByCourseScheduleUPK(Integer termId, String Cno, String Tno);

}
