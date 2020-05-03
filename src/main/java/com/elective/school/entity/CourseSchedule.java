package com.elective.school.entity;

import javax.persistence.Column;
import javax.persistence.EmbeddedId;
import javax.persistence.Entity;

@Entity
public class CourseSchedule {

	@EmbeddedId
	private CourseScheduleUPK upk;
	@Column
	private String time;// 上课时间
	@Column(columnDefinition = "int default 20")
	private Integer num;// 选课人数
	@Column
	private String classroom;// 上课地点
	@Column(columnDefinition = "boolean default false")
	private boolean status;//默认：0；1需修改，0无需修改。
	@Column(columnDefinition = "int default 0")
	private Integer weight;

	public CourseScheduleUPK getUpk() {
		return upk;
	}

	public void setUpk(CourseScheduleUPK upk) {
		this.upk = upk;
	}

	public String getTime() {
		return time;
	}

	public void setTime(String time) {
		this.time = time;
	}

	public Integer getNum() {
		return num;
	}

	public void setNum(Integer num) {
		this.num = num;
	}

	public String getClassroom() {
		return classroom;
	}

	public void setClassroom(String classroom) {
		this.classroom = classroom;
	}

	public boolean isStatus() {
		return status;
	}

	public void setStatus(boolean status) {
		this.status = status;
	}

	public Integer getWeight() {
		return weight;
	}

	public void setWeight(Integer weight) {
		this.weight = weight;
	}

	public CourseSchedule() {
		super();
	}	

	public CourseSchedule(CourseScheduleUPK upk, String time, Integer num, String classroom, boolean status,
			Integer weight) {
		super();
		this.upk = upk;
		this.time = time;
		this.num = num;
		this.classroom = classroom;
		this.status = status;
		this.weight = weight;
	}

	@Override
	public String toString() {
		return "CourseSchedule [upk=" + upk + ", time=" + time + ", num=" + num + ", classroom=" + classroom
				+ ", status=" + status + ", weight=" + weight + "]";
	}

}
