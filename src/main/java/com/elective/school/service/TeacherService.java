package com.elective.school.service;

import java.util.Map;

import com.elective.school.entity.Teacher;

public interface TeacherService {

	public Teacher login(String tno, String password);

	public Map<String, Object> getTeacherInfo(String tno);

}
