package com.elective.school.entity;

import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;

@Entity
public class Term {

	@Id
	@GeneratedValue(strategy = GenerationType.IDENTITY)
	@Column(nullable = false)
	private Integer id;
	@Column
	private String term;
	@Column
	private Integer status; //0：教师开设课程，1：学生选课，2：选课结束，3：开始评分，4：学期结束

	public Integer getId() {
		return id;
	}

	public void setId(Integer id) {
		this.id = id;
	}


	public String getTerm() {
		return term;
	}

	public void setTerm(String term) {
		this.term = term;
	}

	public Integer getStatus() {
		return status;
	}

	public void setStatus(Integer status) {
		this.status = status;
	}

	public Term() {
		super();
		// TODO Auto-generated constructor stub
	}

	public Term(Integer id, String term, Integer status) {
		super();
		this.id = id;
		this.term = term;
		this.status = status;
	}

	@Override
	public String toString() {
		return "Term [id=" + id + ", term=" + term + ", status=" + status + "]";
	}

}
