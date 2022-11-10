package com.sht.dao.impl;

import java.util.List;

import org.hibernate.Session;
import org.hibernate.SessionFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Repository;
import org.springframework.transaction.annotation.Transactional;

import com.sht.dao.MessageDao;
import com.sht.entity.Message;
import com.sht.entity.Page;

@Repository
@Transactional
public class MessageDaoImpl implements MessageDao {

	@Autowired
	private SessionFactory sessionFactory;

	private Session getSession() {
		return sessionFactory.getCurrentSession();
	}

	@Override
	public Message getMessage(Long messageId) {
		Message message = (Message) getSession().createQuery("FROM Message m WHERE m.messageId = :Id")
				.setParameter("Id", messageId).uniqueResult();
		return message;
	}
	
	@Override
	public void saveOrUpdate(Message message) {
		if (message.getMessageId() == null) {
			getSession().save(message);
		} else {
			getSession().createQuery("UPDATE Message m SET m.message = :me WHERE m.messageId = :Id")
					.setParameter("me", message.getMessage()).setParameter("Id", message.getMessageId())
					.executeUpdate();
		}
	}

	@Override
	public void deleteMessage(Long messageId) {
		getSession().createQuery("DELETE Message m WHERE m.messageId = :Id").setParameter("Id", messageId)
				.executeUpdate();
	}

	@Override
	public Long numOfMessages() {
		Long num = (Long) getSession().createQuery("SELECT COUNT(*) FROM Message").uniqueResult();
		return num;
	}
	
	@Override
	public List<Message> getMessages(Page page) {
		List<Message> messages = getSession().createQuery("FROM Message", Message.class)
				.setFirstResult((page.getCurrentPage() - 1) * page.getPageSize()).setMaxResults(page.getPageSize())
				.list();
		return messages;
	}

}
