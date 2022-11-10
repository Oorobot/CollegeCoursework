package com.elective.school.entity;

import java.io.Serializable;

import javax.persistence.Column;
import javax.persistence.Embeddable;

@Embeddable
public class ElectiveUPK implements Serializable {

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;

	@Column(length = 10, nullable = false)
	private String sno;
	@Column(nullable = false)
	private Integer termId;
	@Column(length = 10, nullable = false)
	private String cno;
	@Column(length = 10, nullable = false)
	private String tno;

	public String getSno() {
		return sno;
	}

	public void setSno(String sno) {
		this.sno = sno;
	}

	public Integer getTermId() {
		return termId;
	}

	public void setTermId(Integer termId) {
		this.termId = termId;
	}

	public String getCno() {
		return cno;
	}

	public void setCno(String cno) {
		this.cno = cno;
	}

	public String getTno() {
		return tno;
	}

	public void setTno(String tno) {
		this.tno = tno;
	}

	public ElectiveUPK() {
		super();
	}

	public ElectiveUPK(String sno, Integer termId, String cno, String tno) {
		super();
		this.sno = sno;
		this.termId = termId;
		this.cno = cno;
		this.tno = tno;
	}

	@Override
	public String toString() {
		return "ElectiveUPK [sno=" + sno + ", termId=" + termId + ", cno=" + cno + ", tno=" + tno + "]";
	}

	@Override
	public int hashCode() {
		final int prime = 31;
		int result = 1;
		result = prime * result + ((termId == null) ? 0 : termId.hashCode());
		result = prime * result + ((cno == null) ? 0 : cno.hashCode());
		result = prime * result + ((tno == null) ? 0 : tno.hashCode());
		result = prime * result + ((sno == null) ? 0 : sno.hashCode());
		return result;
	}

	@Override
	public boolean equals(Object obj) {
		if (this == obj)
			return true;
		if (obj == null)
			return false;
		if (getClass() != obj.getClass())
			return false;
		ElectiveUPK other = (ElectiveUPK) obj;
		if (sno == null) {
			if (other.sno != null)
				return false;
		} else if (!sno.equals(other.sno))
			return false;
		if (termId == null) {
			if (other.termId != null)
				return false;
		} else if (!termId.equals(other.termId))
			return false;
		if (cno == null) {
			if (other.cno != null)
				return false;
		} else if (!cno.equals(other.cno))
			return false;
		if (tno == null) {
			if (other.tno != null)
				return false;
		} else if (!tno.equals(other.tno))
			return false;
		return true;
	}

}
