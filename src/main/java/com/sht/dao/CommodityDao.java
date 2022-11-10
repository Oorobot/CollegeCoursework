package com.sht.dao;

import java.util.List;

import com.sht.entity.Commodity;
import com.sht.entity.Message;
import com.sht.entity.Orders;
import com.sht.entity.Page;

public interface CommodityDao {

	// ��ɾ����
	public void saveOrUpdate(Commodity commodity);

	public void updateStatus(Commodity commodity);

	public void deleteCommodity(Long commodityId);

	// ��ҳ��ѯ
	public List<Commodity> getCommodities(Page page);

	public Long numOfCommodities();

	// ģ����ѯ
	public List<Commodity> getCommodities(String name, Page page);

	public Long numOfCommodities(String name);

	// ��Ʒ������Ϣ
	public List<Message> getMessages(Long commodityId, Page page);

	public Long numOfMessages(Long commodityId);

	// ��Ʒ������Ϣ
	public List<Orders> getOrders(Long commodityId, Page page);
	
	public Orders getOrder(Long commodityId);

	public Long numOfOrders(Long commodityId);

}
