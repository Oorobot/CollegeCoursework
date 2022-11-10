package com.sht.entity;

import java.util.Date;

public class Orders {
	private Long orderId;
	private String orderNo;
	private double totalPrice;
	private Date submitTime;
	private Boolean isPay;
	private Boolean isOver;

	private Long commodityId; // 卖家发布的商品ID
	private Long userId; // 买家ID

	public Long getOrderId() {
		return orderId;
	}

	public void setOrderId(Long orderId) {
		this.orderId = orderId;
	}

	public String getOrderNo() {
		return orderNo;
	}

	public void setOrderNo(String orderNo) {
		this.orderNo = orderNo;
	}

	public double getTotalPrice() {
		return totalPrice;
	}

	public void setTotalPrice(double totalPrice) {
		this.totalPrice = totalPrice;
	}

	public Date getSubmitTime() {
		return submitTime;
	}

	public void setSubmitTime(Date submitTime) {
		this.submitTime = submitTime;
	}

	public Boolean getIsPay() {
		if (isPay == null)
			isPay = false;
		return isPay;
	}

	public void setIsPay(Boolean isPay) {
		this.isPay = isPay;
	}

	public Boolean getIsOver() {
		if (isOver == null)
			isOver = false;
		return isOver;
	}

	public void setIsOver(Boolean isOver) {
		this.isOver = isOver;
	}

	public Long getCommodityId() {
		return commodityId;
	}

	public void setCommodityId(Long commodityId) {
		this.commodityId = commodityId;
	}

	public Long getUserId() {
		return userId;
	}

	public void setUserId(Long userId) {
		this.userId = userId;
	}

	public Orders() {
		super();
	}

	public Orders(String orderNo, Long userId, Long commodityId, double totalPrice, Date submitTime) {
		super();
		this.orderNo = orderNo;
		this.totalPrice = totalPrice;
		this.submitTime = submitTime;
		this.userId = userId;
		this.commodityId = commodityId;

	}

	public Orders(Long orderId, String orderNo, double totalPrice, Date submitTime, Boolean isPay, Boolean isOver,
			Long commodityId, Long userId) {
		super();
		this.orderId = orderId;
		this.orderNo = orderNo;
		this.totalPrice = totalPrice;
		this.submitTime = submitTime;
		this.isPay = isPay;
		this.isOver = isOver;
		this.commodityId = commodityId;
		this.userId = userId;
	}

	@Override
	public String toString() {
		return "Orders [orderId=" + orderId + ", orderNo=" + orderNo + ", totalPrice=" + totalPrice + ", submitTime="
				+ submitTime + ", isPay=" + isPay + ", isOver=" + isOver + ", commodityId=" + commodityId + ", userId="
				+ userId + "]";
	}

}
