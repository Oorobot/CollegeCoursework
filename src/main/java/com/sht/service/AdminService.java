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

	// 查看用户信息
	public User getUser(Long userId);

	public List<User> getUsers(Page page);

	// 查看商品信息
	public Commodity getCommodity(Long commodityId);

	public List<Commodity> getCommodities(Page page);

	// 查看订单信息
	public Orders getOrder(Long orderId);

	public List<Orders> getOrders(Page page);

	// 查看评论信息
	public Message getMessage(Long messageId);

	public List<Message> getMessages(Page page);

	// 根据 userId 找到 name。
	public String getBuyerName(Long userId);

	// 根据 commodityId 找到商品名.
	public String getCommodityName(Long commodityId);

	// 根据 commodityId 找到 user 的 name.
	public String getSellerName(Long commodityId);

	// 根据 commodityId 找到商品类名。
	public String getCommodityClassName(Long commodityId);

	// 商品类别信息
	public CommodityClass getCommodityClass(Long commodityClassId);

	public List<CommodityClass> getCommodityClasses(Page page);

}
