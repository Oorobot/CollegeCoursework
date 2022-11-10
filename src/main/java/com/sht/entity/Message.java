package com.sht.entity;

public class Message {
	private Long messageId;
	private String message;

	private Long userId;
	private Long commodityId;

	public Long getMessageId() {
		return messageId;
	}

	public void setMessageId(Long messageId) {
		this.messageId = messageId;
	}

	public String getMessage() {
		return message;
	}

	public void setMessage(String message) {
		this.message = message;
	}

	public Message(String message) {
		super();
		this.message = message;
	}

	public Long getUserId() {
		return userId;
	}

	public void setUserId(Long userId) {
		this.userId = userId;
	}

	public Long getCommodityId() {
		return commodityId;
	}

	public void setCommodityId(Long commodityId) {
		this.commodityId = commodityId;
	}

	public Message() {
		super();
	}

	public Message(Long messageId, String message, Long userId, Long commodityId) {
		super();
		this.messageId = messageId;
		this.message = message;
		this.userId = userId;
		this.commodityId = commodityId;
	}

	@Override
	public String toString() {
		return "Message [messageId=" + messageId + ", message=" + message + ", userId=" + userId + ", commodityId="
				+ commodityId + "]";
	}

}
