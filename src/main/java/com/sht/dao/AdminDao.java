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
	
	// 添加管理员账号
	public void saveOrUpdate(Admin admin);

	// 删除管理员账号
	public void deleteAdmin(Long adminId);

	// 验证账号
	public Long numOfAdmin(String username);

	// 验证账号密码
	public Long numOfAdmin(String username, String password);

	// 用户信息
	public User getUser(Long userId);

	public List<User> getUsers(Page page);

	public Long numOfUsers();

	// 商品信息
	public Commodity getCommodity(Long commodityId);

	public List<Commodity> getCommodities(Page page);

	public Long numOfCommodities();

	// 订单信息
	public Orders getOrder(Long orderId);

	public List<Orders> getOrders(Page page);

	public Long numOfOrders();

	// 评论信息
	public Message getMessage(Long messageId);

	public List<Message> getMessages(Page page);

	public Long numOfMessages();

	// 商品类别信息
	public CommodityClass getCommodityClass(Long commodityClassId);

	public List<CommodityClass> getCommodityClasses(Page page);

	public Long numOfCommodityClasses();

	// 根据 userId 找到 name。
	public String getBuyerName(Long userId);

	// 根据 commodityId 找到商品名.
	public String getCommodityName(Long commodityId);

	// 根据 commodityId 找到 user 的 name.
	public String getSellerName(Long commodityId);

	// 根据 commodityId 找到商品类名。
	public String getCommodityClassName(Long commodityId);

}
