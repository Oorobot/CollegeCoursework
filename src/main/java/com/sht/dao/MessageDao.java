package com.sht.dao;

import java.util.List;

import com.sht.entity.Message;
import com.sht.entity.Page;

public interface MessageDao {
	
	public Message getMessage(Long messageId);
	
	public void saveOrUpdate(Message message);

	public void deleteMessage(Long messageId);

	public Long numOfMessages();
	
	public List<Message> getMessages(Page page);

}
