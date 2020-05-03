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
	
}
