package com.sht.dao;

import java.lang.Long;
import java.util.List;

import com.sht.entity.Commodity;
import com.sht.entity.Message;
import com.sht.entity.Orders;
import com.sht.entity.Page;
import com.sht.entity.User;

public interface UserDao {

	public User getUser(Long userId);

	public void saveOrUpdate(User user);

	public void deleteUser(Long userId);

	public Long numOfUser(String username);
	
	public Long numofUsers();

	public Long idOfUser(String username, String password);

	// ����û���������Ʒ��Ϣ
	public List<Commodity> getCommodities(Long userId, Page page);

	public Long numOfCommodities(Long userId);

	// �鿴�û�����
	public List<Message> getMessages(Long userId, Page page);

	public Long numOfMessages(Long userId);

	// ����û�������Ϣ
	public List<Orders> getOrders(Long userId, Page page);

	public Long numOfOrders(Long userId);

}
