package com.elective.school.service;

import java.util.Map;

import com.elective.school.entity.Student;

public interface StudentService {

	public Student login(String sno, String password);
	
	public Map<String ,Object> select();

}
