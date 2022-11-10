package com.sht.dao.impl;

import java.util.List;

import org.hibernate.Session;
import org.hibernate.SessionFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Repository;
import org.springframework.transaction.annotation.Transactional;

import com.sht.dao.UserDao;
import com.sht.entity.Commodity;
import com.sht.entity.Message;
import com.sht.entity.Orders;
import com.sht.entity.Page;
import com.sht.entity.User;

@Repository
@Transactional
public class UserDaoImpl implements UserDao {

	@Autowired
	private SessionFactory sessionFactory;

	private Session getSession() {
		return sessionFactory.getCurrentSession();
	}

	@Override
	public User getUser(Long userId) {
		User user = (User) getSession().createQuery("FROM User u WHERE u.userId = :Id").setParameter("Id", userId)
				.uniqueResult();
		return user;
	}

	@Override
	public void saveOrUpdate(User user) {
		if (user.getUserId() == null)
			getSession().save(user);
		else {
			getSession().createQuery(
					"UPDATE User u SET u.password = :pw ,u.name = :name, u.phone = :phone, u.sex = :sex, u.address=:address WHERE u.userId = :Id")
					.setParameter("pw", user.getPassword()).setParameter("name", user.getName())
					.setParameter("phone", user.getPhone()).setParameter("sex", user.getSex())
					.setParameter("address", user.getAddress()).setParameter("Id", user.getUserId()).executeUpdate();
		}
	}

	@Override
	public void deleteUser(Long userId) {
		getSession().createQuery("DELETE FROM User u WHERE u.userId = :Id").setParameter("Id", userId).executeUpdate();
	}

	@Override
	public Long numOfUser(String username) {
		Long count = (Long) getSession().createQuery("SELECT COUNT(*) FROM User u WHERE u.username = :un")
				.setParameter("un", username).uniqueResult();
		return count;
	}

	@Override
	public Long idOfUser(String username, String password) {
		Long userId = (Long) getSession()
				.createQuery("SELECT u.userId FROM User u WHERE u.username = :un and u.password = :pw")
				.setParameter("un", username).setParameter("pw", password).uniqueResult();
		return userId;
	}

	@Override
	public List<Commodity> getCommodities(Long userId, Page page) {
		List<Commodity> commodities = getSession().createQuery("FROM Commodity c WHERE c.userId = :Id", Commodity.class)
				.setParameter("Id", userId).setFirstResult((page.getCurrentPage() - 1) * page.getPageSize())
				.setMaxResults(page.getPageSize()).list();
		return commodities;
	}

	@Override
	public Long numOfCommodities(Long userId) {
		Long num = (Long) getSession().createQuery("SELECT COUNT(*) FROM Commodity c WHERE c.userId = :Id")
				.setParameter("Id", userId).uniqueResult();
		return num;
	}

	@Override
	public List<Message> getMessages(Long userId, Page page) {
		List<Message> messages = getSession().createQuery("FROM Message m WHERE m.userId = :Id", Message.class)
				.setParameter("Id", userId).setFirstResult((page.getCurrentPage() - 1) * page.getPageSize())
				.setMaxResults(page.getPageSize()).list();
		return messages;
	}

	@Override
	public Long numOfMessages(Long userId) {
		Long num = (Long) getSession().createQuery("SELECT COUNT(*) FROM Message m WHERE m.userId = :Id")
				.setParameter("Id", userId).uniqueResult();
		return num;
	}

	@Override
	public List<Orders> getOrders(Long userId, Page page) {
		List<Orders> orders = getSession().createQuery("FROM Orders o WHERE o.userId = :Id", Orders.class)
				.setParameter("Id", userId).setFirstResult((page.getCurrentPage() - 1) * page.getPageSize())
				.setMaxResults(page.getPageSize()).list();
		return orders;
	}

	@Override
	public Long numOfOrders(Long userId) {
		Long num = (Long) getSession().createQuery("SELECT COUNT(*) FROM Orders o WHERE o.userId = :Id")
				.setParameter("Id", userId).uniqueResult();
		return num;
	}

	@Override
	public Long numofUsers() {
		Long num = (Long) getSession().createQuery("SELECT COUNT(*) FROM User").uniqueResult();
		return num;
	}

}
