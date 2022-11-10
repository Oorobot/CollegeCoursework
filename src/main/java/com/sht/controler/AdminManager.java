package com.sht.controler;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.ModelAttribute;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RequestParam;

import com.sht.entity.Orders;
import com.sht.dao.CommodityClassDao;
import com.sht.dao.CommodityDao;
import com.sht.dao.MessageDao;
import com.sht.dao.OrdersDao;
import com.sht.dao.UserDao;
import com.sht.entity.Commodity;
import com.sht.entity.CommodityClass;
import com.sht.entity.Message;
import com.sht.entity.Page;
import com.sht.entity.User;
import com.sht.service.AdminService;

@RequestMapping("/admin")
@Controller
public class AdminManager {

	@Autowired
	private AdminService adminService;

	@Autowired
	private UserDao userDao;

	@Autowired
	private CommodityClassDao commodityClassDao;

	@Autowired
	private CommodityDao commodityDao;

	@Autowired
	private OrdersDao ordersDao;

	@Autowired
	private MessageDao messageDao;

	@Autowired
	private Page page;

	@RequestMapping("")
	public String login(@RequestParam(value = "username", required = false) String username,
			@RequestParam(value = "password", required = false) String password) {
		if (adminService.login(username, password)) {
			return "redirect:/admin/users";
		}
		return "login";
	}

	@ModelAttribute
	public void getEntity(@RequestParam(value = "userId", required = false) Long userId,
			@RequestParam(value = "commodityId", required = false) Long commodityId,
			@RequestParam(value = "orderId", required = false) Long orderId,
			@RequestParam(value = "messageId", required = false) Long messageId,
			@RequestParam(value = "commodityClassId", required = false) Long commodityClassId,
			Map<String, Object> map) {
		if (userId != null) {
			User user = adminService.getUser(userId);
			map.put("user", user);
		}
		if (commodityId != null) {
			Commodity commodity = adminService.getCommodity(commodityId);
			map.put("commodity", commodity);
		}
		if (orderId != null) {
			Orders order = adminService.getOrder(orderId);
			map.put("order", order);
		}
		if (messageId != null) {
			Message message = adminService.getMessage(messageId);
			map.put("message", message);
		}
		if (commodityClassId != null) {
			CommodityClass commodityClass = adminService.getCommodityClass(commodityClassId);
			map.put("commodityClass", commodityClass);
		}
	}

	@RequestMapping("/users")
	public String users(Model model, @RequestParam(value = "curPage", required = false) Integer curPage) {
		if (curPage != null)
			page.setCurrentPage(curPage);
		page.setTotalNum(userDao.numofUsers().intValue());
		List<User> users = adminService.getUsers(page);
		model.addAttribute("page", page);
		model.addAttribute("users", users);
		return "users";
	}

	@RequestMapping(value = "/users/{id}", method = RequestMethod.POST)
	public String deleteUser(@PathVariable("id") Long userId) {
		userDao.deleteUser(userId);
		return "redirect:/admin/users";
	}

	@RequestMapping("/commodities")
	public String commodities(Map<String, Object> map,
			@RequestParam(value = "curPage", required = false) Integer curPage) {
		if (curPage != null)
			page.setCurrentPage(curPage);
		page.setTotalNum(commodityDao.numOfCommodities().intValue());
		map.put("page", page);
		map.put("adr", "commodities");

		List<Commodity> commodities = adminService.getCommodities(page);
		map.put("commodities", commodities);
		Map<Long, String> clist = new HashMap<Long, String>();
		List<CommodityClass> cc = commodityClassDao.getCommodityClasses();
		for (CommodityClass c : cc) {
			clist.put(c.getCommodityClassId(), c.getCommodityClassName());
		}
		map.put("clist", clist);

		Map<Long, String> ulist = new HashMap<Long, String>();
		for (Commodity c : commodities) {
			ulist.put(c.getUserId(), adminService.getBuyerName(c.getUserId()));
		}
		map.put("ulist", ulist);

		return "commodities";
	}

	@RequestMapping(value = "/commodity/{id}", method = RequestMethod.GET)
	public String commodity(@RequestParam(value = "curPage", required = false) Integer curPage,
			@PathVariable("id") Long commodityId, Map<String, Object> map) {
		Commodity commodity = adminService.getCommodity(commodityId);
		map.put("commodity", commodity);

		String commodityClassName = adminService.getCommodityClassName(commodityId);
		map.put("commodityClassName", commodityClassName);

		String sellerName = adminService.getSellerName(commodityId);
		map.put("sellerName", sellerName);

		if (curPage != null)
			page.setCurrentPage(curPage);
		page.setTotalNum(commodityDao.numOfMessages(commodityId).intValue());
		map.put("page", page);
		map.put("adr", "admin/commodity/" + commodityId);
		map.put("returnAdr","admin/commodities");

		List<Message> messages = commodityDao.getMessages(commodityId, page);
		map.put("messages", messages);

		Map<Long, String> uMap = new HashMap<Long, String>();
		for (Message m : messages) {
			uMap.put(m.getUserId(), adminService.getBuyerName(m.getUserId()));
		}
		map.put("uMap", uMap);

		return "commodityInfo";
	}

	@RequestMapping("/commodities/search")
	public String searchCommodities(@RequestParam(value = "search") String search,
			@RequestParam(value = "curPage", required = false) Integer curPage, Map<String, Object> map) {

		if (curPage != null)
			page.setCurrentPage(curPage);
		search = search.trim();
		page.setTotalNum(commodityDao.numOfCommodities(search).intValue());
		List<Commodity> commodities = commodityDao.getCommodities(search, page);
		map.put("commodities", commodities);
		map.put("page", page);
		map.put("adr", "commodities/search");
		map.put("search", search);

		Map<Long, String> clist = new HashMap<Long, String>();
		List<CommodityClass> cc = commodityClassDao.getCommodityClasses();
		for (CommodityClass c : cc) {
			clist.put(c.getCommodityClassId(), c.getCommodityClassName());
		}
		map.put("clist", clist);

		Map<Long, String> ulist = new HashMap<Long, String>();
		for (Commodity c : commodities) {
			ulist.put(c.getUserId(), adminService.getBuyerName(c.getUserId()));
		}
		map.put("ulist", ulist);

		return "commodities";
	}

	@RequestMapping("/orders")
	public String orders(Map<String, Object> map, @RequestParam(value = "curPage", required = false) Integer curPage) {
		if (curPage != null)
			page.setCurrentPage(curPage);
		page.setTotalNum(ordersDao.numOfOrders().intValue());
		map.put("page", page);

		List<Orders> orders = adminService.getOrders(page);
		map.put("orders", orders);
		Map<Long, String> buyerMap = new HashMap<Long, String>();
		Map<Long, String> sellerMap = new HashMap<Long, String>();
		Map<Long, String> cMap = new HashMap<Long, String>();
		for (Orders o : orders) {
			buyerMap.put(o.getUserId(), adminService.getBuyerName(o.getUserId()));
			sellerMap.put(o.getCommodityId(), adminService.getSellerName(o.getCommodityId()));
			cMap.put(o.getCommodityId(), adminService.getCommodityName(o.getCommodityId()));
		}
		map.put("buyerMap", buyerMap);
		map.put("sellerMap", sellerMap);
		map.put("cMap", cMap);

		return "orders";
	}

	@RequestMapping("/orders/{id}")
	public String deleteOrder(@PathVariable("id") Long orderId) {
		ordersDao.deleteOrder(orderId);
		return "redirect:/admin/orders";
	}

	@RequestMapping("/messages")
	public String messages(Map<String, Object> map,
			@RequestParam(value = "curPage", required = false) Integer curPage) {
		if (curPage != null)
			page.setCurrentPage(curPage);
		page.setTotalNum(messageDao.numOfMessages().intValue());
		map.put("page", page);

		List<Message> messages = adminService.getMessages(page);
		map.put("messages", messages);
		Map<Long, String> uMap = new HashMap<Long, String>();
		Map<Long, String> cMap = new HashMap<Long, String>();
		for (Message m : messages) {
			uMap.put(m.getUserId(), adminService.getBuyerName(m.getUserId()));
			cMap.put(m.getCommodityId(), adminService.getCommodityName(m.getCommodityId()));
		}
		map.put("uMap", uMap);
		map.put("cMap", cMap);

		return "messages";
	}

	@RequestMapping("/messages/{id}")
	public String deleteMessage(@PathVariable("id") Long messageId) {
		messageDao.deleteMessage(messageId);
		return "redirect:/admin/messages";
	}

	@RequestMapping("/commodityClasses")
	public String commodityClasses(Map<String, Object> map,
			@RequestParam(value = "curPage", required = false) Integer curPage) {
		if (curPage != null)
			page.setCurrentPage(curPage);
		page.setTotalNum(commodityClassDao.numOfcommodityClasses().intValue());
		map.put("page", page);

		List<CommodityClass> commodityClasses = adminService.getCommodityClasses(page);
		map.put("commodityClasses", commodityClasses);
		Map<Long, Long> cclist = new HashMap<Long, Long>();
		for (CommodityClass cc : commodityClasses) {
			cclist.put(cc.getCommodityClassId(), commodityClassDao.numOfSingleCommodities(cc.getCommodityClassId()));
		}

		map.put("cclist", cclist);

		return "commodityClasses";
	}

	@RequestMapping("/commodityClass")
	public String commodityClass(Map<String, Object> map) {
		map.put("commodityClass", new CommodityClass());
		return "commodityClass";
	}

	@RequestMapping("/commodityClass/{id}")
	public String commodityClass(@PathVariable("id") Long commodityClassId, Map<String, Object> map) {
		map.put("commodityClass", adminService.getCommodityClass(commodityClassId));
		return "commodityClass";
	}

	@RequestMapping(value = "/commodityClasses", method = RequestMethod.POST)
	public String saveOrUpdateCommodityClass(CommodityClass commodityClass) {
		commodityClassDao.saveOrUpdate(commodityClass);
		return "redirect:/admin/commodityClasses";
	}

	@RequestMapping("/commodityClasses/{id}")
	public String deleteCommodityClass(@PathVariable("id") Long commodityClassId) {
		commodityClassDao.deleteCommodityClass(commodityClassId);
		return "redirect:/admin/commodityClasses";
	}

}
