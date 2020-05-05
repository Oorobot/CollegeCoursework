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
	@Column
	private String memo;// 备注
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

	public String getMemo() {
		return memo;
	}

	public void setMemo(String memo) {
		this.memo = memo;
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

	public CourseSchedule(CourseScheduleUPK upk, String time, Integer num, String classroom, String memo,
			Integer weight) {
		super();
		this.upk = upk;
		this.time = time;
		this.num = num;
		this.classroom = classroom;
		this.memo = memo;
		this.weight = weight;
	}

	@Override
	public String toString() {
		return "CourseSchedule [upk=" + upk + ", time=" + time + ", num=" + num + ", classroom=" + classroom + ", memo="
				+ memo + ", weight=" + weight + "]";
	}

}
