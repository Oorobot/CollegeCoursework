package com.elective.school.entity;

import java.sql.Date;

import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.Id;

@Entity
public class Teacher {
	@Id
	@Column(length = 10, nullable = false)
	private String tno;
	@Column
	private String password;
	@Column
	private String name;
	@Column(columnDefinition = "boolean default false")
	private boolean sex;
	@Column
	private Date birthday;
	@Column
	private String position;
	@Column
	private double salary;
	@Column(length = 10, nullable = false)
	private String ano;

	public String getTno() {
		return tno;
	}

	public void setTno(String tno) {
		this.tno = tno;
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

	public String getPosition() {
		return position;
	}

	public void setPosition(String position) {
		this.position = position;
	}

	public double getSalary() {
		return salary;
	}

	public void setSalary(double salary) {
		this.salary = salary;
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

	public Teacher(String tno, String password, String name, boolean sex, Date birthday, String position, double salary,
			String ano) {
		super();
		this.tno = tno;
		this.password = password;
		this.name = name;
		this.sex = sex;
		this.birthday = birthday;
		this.position = position;
		this.salary = salary;
		this.ano = ano;
	}

	public Teacher() {
		super();
		// TODO Auto-generated constructor stub
	}

	@Override
	public String toString() {
		return "Teacher [tno=" + tno + ", password=" + password + ", name=" + name + ", sex=" + sex + ", birthday="
				+ birthday + ", position=" + position + ", salary=" + salary + ", ano=" + ano + "]";
	}
	

}
