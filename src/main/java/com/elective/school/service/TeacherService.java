package com.elective.school.service;

import java.util.List;
import java.util.Map;

import com.elective.school.entity.CourseSchedule;
import com.elective.school.entity.CourseScheduleUPK;
import com.elective.school.entity.Teacher;
import com.elective.school.entity.Term;

public interface TeacherService {

	public Teacher login(String tno, String password);

	public Map<String, Object> getTeacherInfo(String tno);

	public Map<String, Object> getCourses();
	
	public List<Term> getTermByStatus(Integer status);
	
	public Map<String,Object> validateCS(String[] cs);

	public Map<String,Object> existCS(CourseScheduleUPK upk);
	
	public Map<String, Object> getCS(Integer termId,String tno);
}
