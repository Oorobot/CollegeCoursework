package com.sht.entity;

public class Commodity {
	private Long commodityId;
	private String commodityName;
	private double commodityPrice;
	private String commodityImage;
	private String commodityDepict;
	private Boolean isOrdered;

	private Long userId;
	private Long commodityClassId;

	public Long getCommodityId() {
		return commodityId;
	}

	public void setCommodityId(Long commodityId) {
		this.commodityId = commodityId;
	}

	public String getCommodityName() {
		return commodityName;
	}

	public void setCommodityName(String commodityName) {
		this.commodityName = commodityName;
	}

	public double getCommodityPrice() {
		return commodityPrice;
	}

	public void setCommodityPrice(double commodityPrice) {
		this.commodityPrice = commodityPrice;
	}

	public String getCommodityImage() {
		return commodityImage;
	}

	public void setCommodityImage(String commodityImage) {
		this.commodityImage = commodityImage;
	}

	public String getCommodityDepict() {
		return commodityDepict;
	}

	public void setCommodityDepict(String commodityDepict) {
		this.commodityDepict = commodityDepict;
	}

	public Long getUserId() {
		return userId;
	}

	public void setUserId(Long userId) {
		this.userId = userId;
	}

	public Long getCommodityClassId() {
		return commodityClassId;
	}

	public void setCommodityClassId(Long commodityClassId) {
		this.commodityClassId = commodityClassId;
	}

	public Boolean getIsOrdered() {
		if (isOrdered == null)
			isOrdered = false;
		return isOrdered;
	}

	public void setIsOrdered(Boolean isOrdered) {
		this.isOrdered = isOrdered;
	}

	public Commodity(String commodityName, double commodityPrice, String commodityImage, String commodityDepict) {
		super();
		this.commodityName = commodityName;
		this.commodityPrice = commodityPrice;
		this.commodityImage = commodityImage;
		this.commodityDepict = commodityDepict;
	}

	public Commodity(Long commodityId, String commodityName, double commodityPrice, String commodityImage,
			String commodityDepict, Long userId, Long commodityClassId, Boolean isOrdered) {
		super();
		this.commodityId = commodityId;
		this.commodityName = commodityName;
		this.commodityPrice = commodityPrice;
		this.commodityImage = commodityImage;
		this.commodityDepict = commodityDepict;
		this.userId = userId;
		this.commodityClassId = commodityClassId;
		this.isOrdered = isOrdered;
	}

	public Commodity() {
		super();
	}

	@Override
	public String toString() {
		return "Commodity [commodityId=" + commodityId + ", commodityName=" + commodityName + ", commodityPrice="
				+ commodityPrice + ", commodityImage=" + commodityImage + ", commodityDepict=" + commodityDepict
				+ ", userId=" + userId + ", commodityClassId=" + commodityClassId + "]";
	}

}
