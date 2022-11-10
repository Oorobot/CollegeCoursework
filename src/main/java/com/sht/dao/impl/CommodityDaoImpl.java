package com.sht.dao.impl;

import java.util.List;

import org.hibernate.Session;
import org.hibernate.SessionFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Repository;
import org.springframework.transaction.annotation.Transactional;

import com.sht.dao.CommodityDao;
import com.sht.entity.Commodity;
import com.sht.entity.Message;
import com.sht.entity.Orders;
import com.sht.entity.Page;

@Repository
@Transactional
public class CommodityDaoImpl implements CommodityDao {

	@Autowired
	private SessionFactory sessionFactory;

	private Session getSession() {
		return sessionFactory.getCurrentSession();
	}

	@Override
	public void saveOrUpdate(Commodity commodity) {
		if (commodity.getCommodityId() == null) {
			getSession().save(commodity);
		} else {
			getSession().createQuery(
					"UPDATE Commodity c SET c.commodityName = :name,c.commodityPrice = :price,c.commodityImage = :image,c.commodityDepict = :depict,c.isOrdered = :iO WHERE c.commodityId = :Id")
					.setParameter("name", commodity.getCommodityName())
					.setParameter("price", commodity.getCommodityPrice())
					.setParameter("image", commodity.getCommodityImage())
					.setParameter("depict", commodity.getCommodityDepict()).setParameter("iO", commodity.getIsOrdered())
					.setParameter("Id", commodity.getCommodityId()).executeUpdate();
		}
	}

	@Override
	public void updateStatus(Commodity commodity) {
		getSession().createQuery("UPDATE Commodity c SET c.isOrdered = :iO WHERE c.commodityId = :Id")
				.setParameter("iO", commodity.getIsOrdered()).setParameter("Id", commodity.getCommodityId())
				.executeUpdate();

	}

	@Override
	public void deleteCommodity(Long commodityId) {
		getSession().createQuery("DELETE FROM Commodity c WHERE c.commodityId = :Id").setParameter("Id", commodityId)
				.executeUpdate();
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
		Long count = (Long) getSession().createQuery("SELECT COUNT(*) FROM Commodity").uniqueResult();
		return count;
	}

	@Override
	public List<Commodity> getCommodities(String name, Page page) {
		List<Commodity> commodities = getSession()
				.createQuery("SELECT c FROM Commodity c WHERE c.commodityName LIKE :cn", Commodity.class)
				.setParameter("cn", "%" + name + "%").setFirstResult((page.getCurrentPage() - 1) * page.getPageSize())
				.setMaxResults(page.getPageSize()).list();
		return commodities;
	}

	@Override
	public Long numOfCommodities(String name) {
		Long num = (Long) getSession().createQuery("SELECT COUNT(*) FROM Commodity c WHERE c.commodityName LIKE :cn")
				.setParameter("cn", "%" + name + "%").uniqueResult();
		return num;
	}

	@Override
	public List<Message> getMessages(Long commodityId, Page page) {
		List<Message> messages = getSession().createQuery("FROM Message m WHERE m.commodityId = :Id", Message.class)
				.setParameter("Id", commodityId).setFirstResult((page.getCurrentPage() - 1) * page.getPageSize())
				.setMaxResults(page.getPageSize()).list();
		return messages;
	}

	@Override
	public Long numOfMessages(Long commodityId) {
		Long num = (Long) getSession().createQuery("SELECT COUNT(*) FROM Message m WHERE m.commodityId = :Id")
				.setParameter("Id", commodityId).uniqueResult();
		return num;
	}

	@Override
	public List<Orders> getOrders(Long commodityId, Page page) {
		List<Orders> orders = getSession().createQuery("FROM Orders o WHERE o.commodityId = :Id", Orders.class)
				.setParameter("Id", commodityId).setFirstResult((page.getCurrentPage() - 1) * page.getPageSize())
				.setMaxResults(page.getPageSize()).list();
		return orders;
	}

	@Override
	public Orders getOrder(Long commodityId) {
		Orders order = getSession().createQuery("FROM Orders o WHERE o.commodityId = :Id", Orders.class)
				.setParameter("Id", commodityId).uniqueResult();
		return order;
	}

	@Override
	public Long numOfOrders(Long commodityId) {
		Long num = (Long) getSession().createQuery("SELECT COUNT(*) FROM Orders o WHERE o.commodityId = :Id")
				.setParameter("Id", commodityId).uniqueResult();
		return num;
	}

}
