package com.sht.controler;

import javax.servlet.http.HttpServletRequest;
import java.io.File;
import java.io.FileOutputStream;

import org.springframework.web.multipart.MultipartFile;

public class ImageUtil {

	public static String saveFile(MultipartFile file, HttpServletRequest request) {

		String pathval = request.getSession().getServletContext().getRealPath("/") + "WEB-INF/";

		String newFileName = String.valueOf(System.currentTimeMillis());

		String saveFilePath = "images/uploadFile";

		File fileDir = new File(pathval + saveFilePath);
		if (!fileDir.exists()) {
			fileDir.mkdirs();
		}

		String filename = file.getOriginalFilename();

		String extensimName = filename.substring(filename.lastIndexOf(".") + 1);
		try {
			String imgPath = saveFilePath + newFileName + "." + extensimName;
			System.out.println(pathval + imgPath);
			FileOutputStream out = new FileOutputStream(pathval + imgPath);
			out.write(file.getBytes());
			out.flush();
			out.close();
			return imgPath;
		} catch (Exception e) {
			e.printStackTrace();
		}
		return null;
	}

	public static void deleteFile(String path) {
		path = "E:\\eclipse-workspace\\.metadata\\.plugins\\org.eclipse.wst.server.core\\tmp0\\wtpwebapps\\SHT\\WEB-INF\\"
				+ path;
		File fileDir = new File(path);
		if (fileDir.exists()) {
			fileDir.delete();
			System.out.println("É¾³ý³É¹¦..");
		}
	}

}
