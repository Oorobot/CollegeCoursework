package com.sht.dao;

import java.util.List;

import com.sht.entity.Orders;
import com.sht.entity.Page;

public interface OrdersDao {

	public Orders getOrder(Long orderId);

	public Orders getOrder(String orderNo);

	public void saveOrUpdate(Orders order);

	public void deleteOrder(Long orderId);

	public Long numOfOrders();

	public List<Orders> getOrders(Page page);
}
