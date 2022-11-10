package com.sht.dao.impl;

import java.util.List;

import org.hibernate.Session;
import org.hibernate.SessionFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Repository;
import org.springframework.transaction.annotation.Transactional;

import com.sht.dao.OrdersDao;
import com.sht.entity.Orders;
import com.sht.entity.Page;

@Repository
@Transactional
public class OrdersDaoImpl implements OrdersDao {

	@Autowired
	private SessionFactory sessionFactory;

	private Session getSession() {
		return sessionFactory.getCurrentSession();
	}

	@Override
	public Orders getOrder(Long orderId) {
		Orders order = (Orders) getSession().createQuery("FROM Orders o WHERE o.orderId = :Id")
				.setParameter("Id", orderId).uniqueResult();
		return order;
	}

	@Override
	public Orders getOrder(String orderNo) {
		Orders order = (Orders) getSession().createQuery("FROM Orders o WHERE o.orderNo = :No")
				.setParameter("No", orderNo).uniqueResult();
		return order;
	}

	@Override
	public void saveOrUpdate(Orders order) {
		if (order.getOrderId() == null) {
			getSession().save(order);
		} else {
			getSession().createQuery("UPDATE Orders o SET o.isPay = :Ip,o.isOver = :Io WHERE o.orderId = :Id")
					.setParameter("Ip", order.getIsPay()).setParameter("Io", order.getIsOver())
					.setParameter("Id", order.getOrderId()).executeUpdate();
		}

	}

	@Override
	public void deleteOrder(Long orderId) {
		getSession().createQuery("DELETE FROM Orders o WHERE o.orderId = :Id").setParameter("Id", orderId)
				.executeUpdate();
	}

	@Override
	public Long numOfOrders() {
		Long num = (Long) getSession().createQuery("SELECT COUNT(*) FROM Orders").uniqueResult();
		return num;
	}

	@Override
	public List<Orders> getOrders(Page page) {
		List<Orders> orders = getSession().createQuery("FROM Orders", Orders.class)
				.setFirstResult((page.getCurrentPage() - 1) * page.getPageSize()).setMaxResults(page.getPageSize())
				.list();
		return orders;
	}
}
