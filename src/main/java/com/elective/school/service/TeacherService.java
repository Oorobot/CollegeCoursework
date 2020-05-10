package com.elective.school.service;

import java.util.List;
import java.util.Map;

import com.elective.school.entity.CourseScheduleUPK;
import com.elective.school.entity.Teacher;
import com.elective.school.entity.Term;

public interface TeacherService {

	public Teacher login(String tno, String password);

	public Map<String, Object> getTeacherInfo(String tno);

	public Map<String, Object> getCourses();

	public List<Term> getTermByStatus(Integer status);

	public Map<String, Object> validateCS(String[] cs);

	public Map<String, Object> existCS(CourseScheduleUPK upk);

	public Map<String, Object> getCS(Integer termId, String tno);

	public Map<String, Object> deleteCS(CourseScheduleUPK upk);

	public Map<String, Object> score(CourseScheduleUPK upk);

	public Map<String, Object> validateScore(String[] usualScore, String[] examScore);

	public Boolean updateScore(String[] usualScore, String[] examScore, String[] sno, String tno,
			String termId, String cno);
}
