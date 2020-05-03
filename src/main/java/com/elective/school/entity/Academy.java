package com.elective.school.entity;

import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.Id;

@Entity
public class Academy {

	@Id
	@Column(length = 10, nullable = false)
	private String ano;
	@Column
	private String name;
	@Column
	private String address;
	@Column
	private String phone;

	public String getAno() {
		return ano;
	}

	public void setAno(String ano) {
		this.ano = ano;
	}

	public String getName() {
		return name;
	}

	public void setName(String name) {
		this.name = name;
	}

	public String getAddress() {
		return address;
	}

	public void setAddress(String address) {
		this.address = address;
	}

	public String getPhone() {
		return phone;
	}

	public void setPhone(String phone) {
		this.phone = phone;
	}

	public Academy() {
		super();
	}

	public Academy(String ano, String name, String address, String phone) {
		super();
		this.ano = ano;
		this.name = name;
		this.address = address;
		this.phone = phone;
	}

	@Override
	public String toString() {
		return "Academy [ano=" + ano + ", name=" + name + ", address=" + address + ", phone=" + phone + "]";
	}

}
