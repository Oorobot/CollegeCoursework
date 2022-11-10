package com.elective.school.entity;

import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.Id;

@Entity
public class Course {

	@Id
	@Column(length = 10, nullable = false)
	private String cno;
	@Column
	private String name;
	@Column
	private Integer credit;//绩点
	@Column
	private Integer hour;//课时
	@Column(length = 10, nullable = false)
	private String ano;

	public String getCno() {
		return cno;
	}

	public void setCno(String cno) {
		this.cno = cno;
	}

	public String getName() {
		return name;
	}

	public void setName(String name) {
		this.name = name;
	}

	public Integer getCredit() {
		return credit;
	}

	public void setCredit(Integer credit) {
		this.credit = credit;
	}

	public Integer getHour() {
		return hour;
	}

	public void setHour(Integer hour) {
		this.hour = hour;
	}

	public String getAno() {
		return ano;
	}

	public void setAno(String ano) {
		this.ano = ano;
	}

	public Course() {
		super();
	}

	public Course(String cno, String name, Integer credit, Integer hour, String ano) {
		super();
		this.cno = cno;
		this.name = name;
		this.credit = credit;
		this.hour = hour;
		this.ano = ano;
	}

	@Override
	public String toString() {
		return "Course [cno=" + cno + ", name=" + name + ", credit=" + credit + ", hour=" + hour + ", ano=" + ano + "]";
	}
	
	
}
