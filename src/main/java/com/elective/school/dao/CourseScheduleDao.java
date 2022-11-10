package com.elective.school.dao;

import java.util.List;

import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.Query;

import com.elective.school.entity.CourseSchedule;
import com.elective.school.entity.CourseScheduleUPK;

public interface CourseScheduleDao extends JpaRepository<CourseSchedule, CourseScheduleUPK> {

	@Query(value = "Select cs From CourseSchedule cs Where cs.upk.cno = ?1")
	public List<CourseSchedule> findByCno(String Cno);

	@Query(value = "Select cs From CourseSchedule cs Where cs.upk.tno = ?1")
	public List<CourseSchedule> findByTno(String Tno);

	@Query(value = "Select cs From CourseSchedule cs Where cs.upk.termId = ?1")
	public List<CourseSchedule> findByTermId(Integer termId);

	@Query(value = "Select cs From CourseSchedule cs Where cs.upk.termId = ?1 and cs.upk.tno = ?2")
	public List<CourseSchedule> findByTermIdAndTno(Integer termId, String tno);

	@Query(value = "Select cs From CourseSchedule cs Where cs.upk.termId = ?1 and "
			+ "cs.upk.tno in (Select t.tno from Teacher t Where t.tno like %?2% and t.name like %?3%) and "
			+ "cs.upk.cno in (Select c.cno from Course c where c.cno like %?4% and c.name like %?5%)")
	public List<CourseSchedule> findByTermIdLikeTnoAndCno(Integer termId, String tno, String tname, String cno,
			String cname);
}
