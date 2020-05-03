package com.elective.school.util;

import java.security.MessageDigest;
import java.util.Base64;

public class MD5 {

	static public String md5Encoding(String message) {
		String temp = "";
		try {
			MessageDigest digest = MessageDigest.getInstance("MD5");
			byte[] bytes = digest.digest(message.getBytes("UTF-8"));
			temp = Base64.getEncoder().encodeToString(bytes);
		} catch (Exception e) {
			e.printStackTrace();
		}
		return temp;
	}

}
