package com.elective.school.util;

import java.util.Collections;
import java.util.HashMap;
import java.util.Map;

public class Constant {
	public static Map<Integer, String> term_status = new HashMap<Integer, String>();
	static {
		Map<Integer, String> a = new HashMap<Integer, String>();
		a.put(0, "学期开始");
		a.put(1, "选课阶段");
		a.put(2, "选课结束");
		a.put(3, "教师评分");
		a.put(4, "学期结束");
		term_status = Collections.unmodifiableMap(a);
	}
	public static Map<Integer, String> sex = new HashMap<Integer, String>();
	static {
		Map<Integer, String> a = new HashMap<Integer, String>();
		a.put(0, "男");
		a.put(1, "女");
		sex = Collections.unmodifiableMap(a);
	}
	// category: 0:term;

	// 正则表达式
	public static final String reg_num = "^(\\d)+$";
	public static final String reg_double = "^\\d+(.\\d+)?$";
	public static final String reg_term_status = "^(\\d){4}-(\\d){4}[春夏秋冬]季$";
	public static final String reg_academy_ano = "^(\\d){2}$";
	public static final String reg_course_cno = "^(\\d){8}$";
	public static final String reg_student_sno = "^(\\d){4}$";
	public static final String reg_teacher_tno = "^(\\d){4}$";
	public static final String reg_date = "^(\\d{4})-((0?([1-9]))|(1[1|2]))-((0?[1-9])|([12]([1-9]))|(3[0|1]))$";
}
