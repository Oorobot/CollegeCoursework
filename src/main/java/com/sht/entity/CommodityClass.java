package com.sht.entity;

public class CommodityClass {
	private Long commodityClassId;
	private String commodityClassName;

	public Long getCommodityClassId() {
		return commodityClassId;
	}

	public void setCommodityClassId(Long commodityClassId) {
		this.commodityClassId = commodityClassId;
	}

	public String getCommodityClassName() {
		return commodityClassName;
	}

	public void setCommodityClassName(String commodityClassName) {
		this.commodityClassName = commodityClassName;
	}

	public CommodityClass() {
		super();
	}

	public CommodityClass(String commodityClassName) {
		super();
		this.commodityClassName = commodityClassName;
	}

	@Override
	public String toString() {
		return "ConmodityClass [commodityClassId=" + commodityClassId + ", commodityClassName=" + commodityClassName
				+ "]";
	}

}
