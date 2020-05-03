package com.elective.school.entity;

import javax.persistence.Column;
import javax.persistence.EmbeddedId;
import javax.persistence.Entity;

@Entity
public class Elective {

	@EmbeddedId
	private ElectiveUPK upk;
	@Column
	private Integer usualScore;
	@Column
	private Integer examScore;
	@Column
	private Integer totalScore;

	public ElectiveUPK getUpk() {
		return upk;
	}

	public void setUpk(ElectiveUPK upk) {
		this.upk = upk;
	}

	public Integer getUsualScore() {
		return usualScore;
	}

	public void setUsualScore(Integer usualScore) {
		this.usualScore = usualScore;
	}

	public Integer getExamScore() {
		return examScore;
	}

	public void setExamScore(Integer examScore) {
		this.examScore = examScore;
	}

	public Integer getTotalScore() {
		return totalScore;
	}

	public void setTotalScore(Integer totalScore) {
		this.totalScore = totalScore;
	}

	public Elective() {
		super();
	}

	public Elective(ElectiveUPK upk, Integer usualScore, Integer examScore, Integer totalScore) {
		super();
		this.upk = upk;
		this.usualScore = usualScore;
		this.examScore = examScore;
		this.totalScore = totalScore;
	}

	@Override
	public String toString() {
		return "Elective [upk=" + upk + ", usualScore=" + usualScore + ", examScore=" + examScore
				+ ", totalScore=" + totalScore + "]";
	}

}
