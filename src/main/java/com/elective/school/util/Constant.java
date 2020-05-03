package com.elective.school.util;

import java.util.Collections;
import java.util.HashMap;
import java.util.Map;

public class Constant {
	public static Map<Integer, String> term_status = new HashMap<Integer, String>();
	static {
		Map<Integer, String> a = new HashMap<Integer, String>();
		a.put(0, "学期开始，教师开设课程。");
		a.put(1, "学生选课");
		a.put(2, "选课结束");
		a.put(3, "教师评分");
		a.put(4, "学期结束");
		term_status = Collections.unmodifiableMap(a);
	}
	public static Map<Integer, String> sex = new HashMap<Integer, String>();
		static{
			Map<Integer, String> a = new HashMap<Integer, String>();
			a.put(0, "男");
			a.put(1, "女");
			sex = Collections.unmodifiableMap(a);
		}
	// category: 0:term;
		
	//正则表达式
	public static final String reg_term_status = "^(\\d){4}-(\\d){4}[春夏秋冬]季$";
}
