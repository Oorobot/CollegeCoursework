package com.sht.dao.impl;

import java.util.List;

import org.hibernate.Session;
import org.hibernate.SessionFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Repository;

import com.sht.dao.AdminDao;
import com.sht.entity.Admin;
import com.sht.entity.Commodity;
import com.sht.entity.CommodityClass;
import com.sht.entity.Message;
import com.sht.entity.Orders;
import com.sht.entity.Page;
import com.sht.entity.User;

@Repository
public class AdminDaoImpl implements AdminDao {

	@Autowired
	private SessionFactory sessionFactory;

	private Session getSession() {
		return sessionFactory.getCurrentSession();
	}

	@Override
	public void saveOrUpdate(Admin admin) {
		if (admin.getAdminId() == null)
			getSession().save(admin);
		else
			getSession().createQuery("UPDATE Admin a SET a.password = :pw WHERE a.adminId = :Id")
					.setParameter("pw", admin.getPassword()).setParameter("Id", admin.getAdminId()).executeUpdate();
	}

	@Override
	public void deleteAdmin(Long adminId) {
		getSession().createQuery("DELETE FROM Admin a where a.adminId = ?0").setParameter(0, adminId).executeUpdate();
	}

	@Override
	public Long numOfAdmin(String username) {
		Long count = (Long) getSession().createQuery("SELECT COUNT(*) FROM Admin a WHERE a.username = ?0")
				.setParameter(0, username).uniqueResult();
		return count;
	}

	@Override
	public Long numOfAdmin(String username, String password) {
		Long count = (Long) getSession()
				.createQuery("SELECT COUNT(*) FROM Admin a WHERE a.username = ?0" + " AND a.password = ?1")
				.setParameter(0, username).setParameter(1, password).uniqueResult();
		return count;
	}

	@Override
	public User getUser(Long userId) {
		User user = (User) getSession().createQuery("FROM User u WHERE u.userId = :Id").setParameter("Id", userId)
				.uniqueResult();
		return user;
	}

	@Override
	public List<User> getUsers(Page page) {
		List<User> users = getSession().createQuery("FROM User", User.class)
				.setFirstResult((page.getCurrentPage() - 1) * page.getPageSize()).setMaxResults(page.getPageSize())
				.list();
		return users;
	}

	@Override
	public Long numOfUsers() {
		Long num = (Long) getSession().createQuery("SELECT COUNT(*) FROM User").uniqueResult();
		return num;
	}

	@Override
	public Commodity getCommodity(Long commodityId) {
		Commodity commodity = (Commodity) getSession().createQuery("FROM Commodity c WHERE c.commodityId = :Id")
				.setParameter("Id", commodityId).uniqueResult();
		return commodity;
	}

	@Override
	public List<Commodity> getCommodities(Page page) {
		List<Commodity> commodities = getSession().createQuery("FROM Commodity", Commodity.class)
				.setFirstResult((page.getCurrentPage() - 1) * page.getPageSize()).setMaxResults(page.getPageSize())
				.list();
		return commodities;
	}

	@Override
	public Long numOfCommodities() {
		Long num = (Long) getSession().createQuery("SELECT COUNT(*) FROM Commodity").uniqueResult();
		return num;
	}

	@Override
	public Orders getOrder(Long orderId) {
		Orders order = (Orders) getSession().createQuery("FROM Orders o WHERE o.orderId = :Id")
				.setParameter("Id", orderId).uniqueResult();
		return order;
	}

	@Override
	public List<Orders> getOrders(Page page) {
		List<Orders> orders = getSession().createQuery("FROM Orders", Orders.class)
				.setFirstResult((page.getCurrentPage() - 1) * page.getPageSize()).setMaxResults(page.getPageSize())
				.list();
		return orders;
	}

	@Override
	public Long numOfOrders() {
		Long num = (Long) getSession().createQuery("SELECT COUNT(*) FROM Orders").uniqueResult();
		return num;
	}

	@Override
	public Message getMessage(Long messageId) {
		Message message = (Message) getSession().createQuery("FROM Message m WHERE m.messageId = :Id")
				.setParameter("Id", messageId).uniqueResult();
		return message;
	}

	@Override
	public List<Message> getMessages(Page page) {
		List<Message> messages = getSession().createQuery("FROM Message", Message.class)
				.setFirstResult((page.getCurrentPage() - 1) * page.getPageSize()).setMaxResults(page.getPageSize())
				.list();
		return messages;
	}

	@Override
	public Long numOfMessages() {
		Long num = (Long) getSession().createQuery("SELECT COUNT(*) FROM Message").uniqueResult();
		return num;
	}

	@Override
	public String getCommodityClassName(Long commodityId) {
		String name = (String) getSession().createQuery(
				"SELECT cc.commodityClassName FROM CommodityClass cc WHERE cc.commodityClassId IN (SELECT c.commodityClassId FROM Commodity c WHERE c.commodityId = :Id) ")
				.setParameter("Id", commodityId).uniqueResult();
		return name;
	}

	@Override
	public CommodityClass getCommodityClass(Long commodityClassId) {
		CommodityClass commodityClass = (CommodityClass) getSession()
				.createQuery("FROM CommodityClass cc WHERE cc.commodityClassId = :Id")
				.setParameter("Id", commodityClassId).uniqueResult();
		return commodityClass;
	}

	@Override
	public List<CommodityClass> getCommodityClasses(Page page) {
		List<CommodityClass> commodityClasses = getSession().createQuery("FROM CommodityClass", CommodityClass.class)
				.setFirstResult((page.getCurrentPage() - 1) * page.getPageSize()).setMaxResults(page.getPageSize())
				.list();
		return commodityClasses;
	}

	@Override
	public String getBuyerName(Long userId) {
		String name = (String) getSession().createQuery("SELECT u.name FROM User u WHERE u.userId = :Id")
				.setParameter("Id", userId).uniqueResult();
		return name;
	}

	@Override
	public String getCommodityName(Long commodityId) {
		String name = (String) getSession()
				.createQuery("SELECT c.commodityName FROM Commodity c WHERE c.commodityId = :Id")
				.setParameter("Id", commodityId).uniqueResult();
		return name;
	}

	@Override
	public String getSellerName(Long commodityId) {
		String name = (String) getSession().createQuery(
				"SELECT u.name FROM User u WHERE u.userId IN (SELECT c.userId FROM Commodity c WHERE c.commodityId = :Id) ")
				.setParameter("Id", commodityId).uniqueResult();
		return name;
	}

	@Override
	public Long numOfCommodityClasses() {
		Long num = (Long) getSession().createQuery("SELECT COUNT(*) FROM CommodityClass").uniqueResult();
		return num;
	}

}
