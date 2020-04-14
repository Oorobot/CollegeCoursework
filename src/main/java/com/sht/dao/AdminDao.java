package com.sht.dao;

import java.util.List;

import com.sht.entity.Admin;
import com.sht.entity.Commodity;
import com.sht.entity.CommodityClass;
import com.sht.entity.Message;
import com.sht.entity.Orders;
import com.sht.entity.Page;
import com.sht.entity.User;

public interface AdminDao {
	
	// ��ӹ���Ա�˺�
	public void saveOrUpdate(Admin admin);

	// ɾ������Ա�˺�
	public void deleteAdmin(Long adminId);

	// ��֤�˺�
	public Long numOfAdmin(String username);

	// ��֤�˺�����
	public Long numOfAdmin(String username, String password);

	// �û���Ϣ
	public User getUser(Long userId);

	public List<User> getUsers(Page page);

	public Long numOfUsers();

	// ��Ʒ��Ϣ
	public Commodity getCommodity(Long commodityId);

	public List<Commodity> getCommodities(Page page);

	public Long numOfCommodities();

	// ������Ϣ
	public Orders getOrder(Long orderId);

	public List<Orders> getOrders(Page page);

	public Long numOfOrders();

	// ������Ϣ
	public Message getMessage(Long messageId);

	public List<Message> getMessages(Page page);

	public Long numOfMessages();

	// ��Ʒ�����Ϣ
	public CommodityClass getCommodityClass(Long commodityClassId);

	public List<CommodityClass> getCommodityClasses(Page page);

	public Long numOfCommodityClasses();

	// ���� userId �ҵ� name��
	public String getBuyerName(Long userId);

	// ���� commodityId �ҵ���Ʒ��.
	public String getCommodityName(Long commodityId);

	// ���� commodityId �ҵ� user �� name.
	public String getSellerName(Long commodityId);

	// ���� commodityId �ҵ���Ʒ������
	public String getCommodityClassName(Long commodityId);

}
