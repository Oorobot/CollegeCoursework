package com.elective.school.service;

import java.util.Map;

import com.elective.school.entity.ElectiveUPK;
import com.elective.school.entity.Student;

public interface StudentService {

	public Student login(String sno, String password);

	public Map<String, Object> getTermOnSelect();

	public Map<String, Object> select(String termId, String cno, String cname, String tno, String tname);

	public Map<String, Object> saveElective(ElectiveUPK upk);

	public Map<String, Object> deleteElective(ElectiveUPK upk);

	public Map<String, Object> drop(String termId, String sno);

	public Map<String, Object> score(String termId, String sno);

}
