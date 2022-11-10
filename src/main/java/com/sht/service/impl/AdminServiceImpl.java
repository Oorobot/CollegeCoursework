package com.sht.service.impl;

import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.sht.dao.AdminDao;
import com.sht.entity.Admin;
import com.sht.entity.Commodity;
import com.sht.entity.CommodityClass;
import com.sht.entity.Message;
import com.sht.entity.Orders;
import com.sht.entity.Page;
import com.sht.entity.User;
import com.sht.service.AdminService;

@Service
public class AdminServiceImpl implements AdminService {

	@Autowired
	private AdminDao adminDao;

	@Override
	public Boolean saveOrUpdate(Admin admin) {
		Long count = adminDao.numOfAdmin(admin.getUsername());
		if (count == 0) {
			adminDao.saveOrUpdate(admin);
			return true;
		}
		return false;
	}

	@Override
	public void deleteAdmin(Long adminId) {
		adminDao.deleteAdmin(adminId);
	}

	@Override
	public Boolean login(String username, String password) {
		Long count = adminDao.numOfAdmin(username, password);
		if (count == 0)
			return false;
		return true;
	}

	@Override
	public List<User> getUsers(Page page) {
		page.setTotalNum(adminDao.numOfUsers().intValue());
		return adminDao.getUsers(page);
	}

	@Override
	public List<Commodity> getCommodities(Page page) {
		page.setTotalNum(adminDao.numOfCommodities().intValue());
		return adminDao.getCommodities(page);
	}

	@Override
	public List<Orders> getOrders(Page page) {
		page.setTotalNum(adminDao.numOfOrders().intValue());
		return adminDao.getOrders(page);
	}

	@Override
	public List<Message> getMessages(Page page) {
		page.setTotalNum(adminDao.numOfMessages().intValue());
		return adminDao.getMessages(page);
	}

	@Override
	public String getBuyerName(Long userId) {
		return adminDao.getBuyerName(userId);
	}

	@Override
	public String getCommodityName(Long commodityId) {
		return adminDao.getCommodityName(commodityId);
	}

	@Override
	public String getSellerName(Long commodityId) {
		return adminDao.getSellerName(commodityId);
	}

	@Override
	public String getCommodityClassName(Long commodityId) {
		return adminDao.getCommodityClassName(commodityId);
	}

	@Override
	public User getUser(Long userId) {
		return adminDao.getUser(userId);
	}

	@Override
	public Commodity getCommodity(Long commodityId) {
		return adminDao.getCommodity(commodityId);
	}

	@Override
	public Orders getOrder(Long orderId) {
		return adminDao.getOrder(orderId);
	}

	@Override
	public Message getMessage(Long messageId) {
		return adminDao.getMessage(messageId);
	}

	@Override
	public CommodityClass getCommodityClass(Long commodityClassId) {
		return adminDao.getCommodityClass(commodityClassId);
	}

	@Override
	public List<CommodityClass> getCommodityClasses(Page page) {
		page.setTotalNum(adminDao.numOfCommodityClasses().intValue());
		return adminDao.getCommodityClasses(page);
	}

}