package com.sht.service;

import java.util.List;

import com.sht.entity.Admin;
import com.sht.entity.Commodity;
import com.sht.entity.CommodityClass;
import com.sht.entity.Message;
import com.sht.entity.Orders;
import com.sht.entity.Page;
import com.sht.entity.User;

public interface AdminService {

	public Boolean saveOrUpdate(Admin admin);

	public void deleteAdmin(Long adminId);

	public Boolean login(String username, String password);

	// �鿴�û���Ϣ
	public User getUser(Long userId);

	public List<User> getUsers(Page page);

	// �鿴��Ʒ��Ϣ
	public Commodity getCommodity(Long commodityId);

	public List<Commodity> getCommodities(Page page);

	// �鿴������Ϣ
	public Orders getOrder(Long orderId);

	public List<Orders> getOrders(Page page);

	// �鿴������Ϣ
	public Message getMessage(Long messageId);

	public List<Message> getMessages(Page page);

	// ���� userId �ҵ� name��
	public String getBuyerName(Long userId);

	// ���� commodityId �ҵ���Ʒ��.
	public String getCommodityName(Long commodityId);

	// ���� commodityId �ҵ� user �� name.
	public String getSellerName(Long commodityId);

	// ���� commodityId �ҵ���Ʒ������
	public String getCommodityClassName(Long commodityId);

	// ��Ʒ�����Ϣ
	public CommodityClass getCommodityClass(Long commodityClassId);

	public List<CommodityClass> getCommodityClasses(Page page);

}
