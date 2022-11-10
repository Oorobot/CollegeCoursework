package com.elective.school.entity;

import java.sql.Date;

import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.Id;

@Entity
public class Student {
	@Id
	@Column(length = 10, nullable = false)
	private String sno;
	@Column
	private String password;
	@Column
	private String name;
	@Column(columnDefinition = "boolean default false")
	private boolean sex;
	@Column
	private Date birthday;
	@Column
	private String hometown;
	@Column
	private String phone;
	@Column(length = 10, nullable = false)
	private String ano;

	public String getSno() {
		return sno;
	}

	public void setSno(String sno) {
		this.sno = sno;
	}

	public String getName() {
		return name;
	}

	public void setName(String name) {
		this.name = name;
	}

	public boolean isSex() {
		return sex;
	}

	public void setSex(boolean sex) {
		this.sex = sex;
	}

	public Date getBirthday() {
		return birthday;
	}

	public void setBirthday(Date birthday) {
		this.birthday = birthday;
	}

	public String getHometown() {
		return hometown;
	}

	public void setHometown(String hometown) {
		this.hometown = hometown;
	}

	public String getPhone() {
		return phone;
	}

	public void setPhone(String phone) {
		this.phone = phone;
	}

	public String getAno() {
		return ano;
	}

	public void setAno(String ano) {
		this.ano = ano;
	}

	public String getPassword() {
		return password;
	}

	public void setPassword(String password) {
		this.password = password;
	}

	public Student(String sno, String password, String name, boolean sex, Date birthday, String hometown, String phone,
			String ano) {
		super();
		this.sno = sno;
		this.password = password;
		this.name = name;
		this.sex = sex;
		this.birthday = birthday;
		this.hometown = hometown;
		this.phone = phone;
		this.ano = ano;
	}

	public Student() {
		super();
	}

	@Override
	public String toString() {
		return "Student [sno=" + sno + ", password=" + password + ", name=" + name + ", sex=" + sex + ", birthday="
				+ birthday + ", hometown=" + hometown + ", phone=" + phone + ", ano=" + ano + "]";
	}
	
	
	
}
