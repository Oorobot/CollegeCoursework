package com.sht.dao;

import java.util.List;

import com.sht.entity.Commodity;
import com.sht.entity.Message;
import com.sht.entity.Orders;
import com.sht.entity.Page;

public interface CommodityDao {

	// 添、删、改
	public void saveOrUpdate(Commodity commodity);

	public void updateStatus(Commodity commodity);

	public void deleteCommodity(Long commodityId);

	// 分页查询
	public List<Commodity> getCommodities(Page page);

	public Long numOfCommodities();

	// 模糊查询
	public List<Commodity> getCommodities(String name, Page page);

	public Long numOfCommodities(String name);

	// 商品留言信息
	public List<Message> getMessages(Long commodityId, Page page);

	public Long numOfMessages(Long commodityId);

	// 商品订单信息
	public List<Orders> getOrders(Long commodityId, Page page);
	
	public Orders getOrder(Long commodityId);

	public Long numOfOrders(Long commodityId);

}
