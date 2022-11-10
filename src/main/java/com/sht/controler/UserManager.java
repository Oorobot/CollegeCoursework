package com.sht.controler;

import java.util.Date;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpSession;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.ModelAttribute;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.SessionAttributes;
import org.springframework.web.bind.support.SessionStatus;
import org.springframework.web.multipart.MultipartFile;
import org.springframework.web.servlet.ModelAndView;

import com.sht.dao.CommodityClassDao;
import com.sht.dao.CommodityDao;
import com.sht.dao.MessageDao;
import com.sht.dao.OrdersDao;
import com.sht.dao.UserDao;
import com.sht.entity.Commodity;
import com.sht.entity.CommodityClass;
import com.sht.entity.Message;
import com.sht.entity.Orders;
import com.sht.entity.Page;
import com.sht.entity.User;
import com.sht.service.AdminService;

@SessionAttributes(value = { "userId", "name" })
@Controller
public class UserManager {

	@Autowired
	private Page page;

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

	// 登录
	@RequestMapping("/login")
	public String login(ModelAndView mv) {
		return "login";
	}

	// 退出
	@RequestMapping("/logout")
	public String logout(SessionStatus sessionStatus, HttpSession session) {
		sessionStatus.setComplete();
		return "login";
	}

	// 注册
	@RequestMapping("/register")
	public String register(Map<String, Object> map) {
		map.put("user", new User());
		return "register";
	}

	@RequestMapping(value = "/successR", method = RequestMethod.POST)
	public String register(User user, Map<String, Object> map) {
		userDao.saveOrUpdate(user);
		map.put("msg", "注册成功！！！");
		map.put("msg1", "返回登录页面");
		map.put("adr", "login");
		return "success";
	}

	@ModelAttribute
	public void getEntity(@RequestParam(value = "userId", required = false) Long userId,
			@RequestParam(value = "commodityId", required = false) Long commodityId,
			@RequestParam(value = "orderId", required = false) Long orderId,
			@RequestParam(value = "messageId", required = false) Long messageId, Map<String, Object> map) {
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
	}

	// 首页,商品页面
	@RequestMapping("/home")
	public String home(@RequestParam(value = "username", required = false) String username,
			@RequestParam(value = "password", required = false) String password,
			@RequestParam(value = "curPage", required = false) Integer curPage, Map<String, Object> map,
			HttpSession session) {
		Long userId = userDao.idOfUser(username, password);
		if (userId == null) {
			userId = (Long) session.getAttribute("userId");
			if (userId == null)
				return "login";
		}
		map.put("userId", userId);

		String name = adminService.getBuyerName(userId);
		map.put("name", name);

		if (curPage != null)
			page.setCurrentPage(curPage);
		page.setTotalNum(commodityDao.numOfCommodities().intValue());

		List<Commodity> commodities = adminService.getCommodities(page);
		map.put("commodities", commodities);
		map.put("page", page);
		map.put("adr", "home");

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

		return "home";
	}

	@RequestMapping("/home/{id}")
	public String classCommodities(@PathVariable(value = "id") Long id,
			@RequestParam(value = "curPage", required = false) Integer curPage, Map<String, Object> map) {
		if (curPage != null)
			page.setCurrentPage(curPage);
		page.setTotalNum(commodityClassDao.numOfSingleCommodities(id).intValue());
		List<Commodity> commodities = commodityClassDao.getCommodities(id, page);
		map.put("commodities", commodities);
		map.put("page", page);
		map.put("adr", "home/" + id);

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

		return "home";
	}

	@RequestMapping("/home/search")
	public String searchCommodities(@RequestParam(value = "search") String search,
			@RequestParam(value = "curPage", required = false) Integer curPage, Map<String, Object> map) {
		if (curPage != null)
			page.setCurrentPage(curPage);

		search = search.trim();
		page.setTotalNum(commodityDao.numOfCommodities(search).intValue());
		List<Commodity> commodities = commodityDao.getCommodities(search, page);
		map.put("commodities", commodities);
		map.put("page", page);
		map.put("adr", "home/search");
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

		return "home";
	}

	// 我的商品
	@RequestMapping("/myCommodities")
	public String myCommodities(@RequestParam(value = "curPage", required = false) Integer curPage, HttpSession session,
			Map<String, Object> map) {
		if (curPage != null)
			page.setCurrentPage(curPage);

		Long userId = (Long) session.getAttribute("userId");
		page.setTotalNum(userDao.numOfCommodities(userId).intValue());
		List<Commodity> commodities = userDao.getCommodities(userId, page);
		map.put("commodities", commodities);
		map.put("page", page);

		Map<Long, String> ccMap = new HashMap<Long, String>();
		List<CommodityClass> cc = commodityClassDao.getCommodityClasses();
		for (CommodityClass c : cc) {
			ccMap.put(c.getCommodityClassId(), c.getCommodityClassName());
		}
		map.put("ccMap", ccMap);

		return "myCommodities";
	}

	// 我的商品订单
	@RequestMapping("/myCommodityOrder/{id}")
	public String myCommodityOrder(@PathVariable("id") Long commodityId, Map<String, Object> map, HttpSession session) {
		Orders order = commodityDao.getOrder(commodityId);
		User user = adminService.getUser(order.getUserId());
		map.put("order", order);
		map.put("user", user);
		return "myCommodityOrder";
	}

	// 发布商品
	@RequestMapping("/newCommodity")
	public String newCommodity(Map<String, Object> map) {
		map.put("commodity", new Commodity());

		Map<Long, String> ccMap = new HashMap<Long, String>();
		List<CommodityClass> cc = commodityClassDao.getCommodityClasses();
		for (CommodityClass c : cc) {
			ccMap.put(c.getCommodityClassId(), c.getCommodityClassName());
		}
		map.put("ccMap", ccMap);

		return "sOuCommodity";
	}

	// 我的商品中修改商品信息
	@RequestMapping("/myCommodities/{id}")
	public String myCommodities(@PathVariable("id") Long commodityId, Map<String, Object> map) {

		Map<Long, String> ccMap = new HashMap<Long, String>();
		List<CommodityClass> cc = commodityClassDao.getCommodityClasses();
		for (CommodityClass c : cc) {
			ccMap.put(c.getCommodityClassId(), c.getCommodityClassName());
		}
		map.put("ccMap", ccMap);

		Commodity commodity = adminService.getCommodity(commodityId);
		map.put("oldImage", commodity.getCommodityImage());
		map.put("commodity", commodity);

		return "sOuCommodity";
	}

	// 保存或修改商品信息
	@RequestMapping(value = "/successC", method = RequestMethod.POST)
	public String saveOrUpdateCommodity(@RequestParam(value = "file") MultipartFile file,
			@RequestParam(value = "oldImage") String oldImage, HttpServletRequest request, HttpSession session,
			Commodity commodity, Map<String, Object> map) {
		if (file.getSize() != 0) {
			ImageUtil.deleteFile(oldImage);
			String path = ImageUtil.saveFile(file, request);
			commodity.setCommodityImage(path);
		}
		if (commodity.getCommodityId() == null) {
			commodity.setUserId((Long) session.getAttribute("userId"));
		}
		commodityDao.saveOrUpdate(commodity);

		map.put("msg", "操作成功！！！");
		map.put("msg1", "返回首页");
		map.put("adr", "home");
		return "success";
	}

	// 查看商品详情
	@RequestMapping("/home/commodity/{id}")
	public String commodityInfo(@RequestParam(value = "curPage", required = false) Integer curPage,
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
		map.put("adr", "home/commodity/" + commodityId);
		map.put("returnAdr", "home");

		List<Message> messages = commodityDao.getMessages(commodityId, page);
		map.put("messages", messages);

		Map<Long, String> uMap = new HashMap<Long, String>();
		for (Message m : messages) {
			uMap.put(m.getUserId(), adminService.getBuyerName(m.getUserId()));
		}
		map.put("uMap", uMap);

		return "commodityInfo";
	}

	@RequestMapping(value = "/home/commodity/{Id}", method = RequestMethod.POST)
	public String addMessage(@RequestParam("mge") String mge, @PathVariable("Id") Long commodityId,
			Map<String, Object> map, HttpSession session) {
		Long userId = (Long) session.getAttribute("userId");
		Message message = new Message();
		message.setUserId(userId);
		message.setMessage(mge);
		message.setCommodityId(commodityId);
		messageDao.saveOrUpdate(message);

		String re = "redirect:/home/commodity/" + commodityId + "?curPage=1";
		System.out.println(re);
		return re;
	}

	// 在商品信息详情页中点击购买产生订单
	@RequestMapping(value = "/home/buy", method = RequestMethod.POST)
	public String buyCommodity(@RequestParam("c_cId") Long commodityId, @RequestParam("c_price") double price,
			HttpSession session, Map<String, Object> map) {
		String orderNo = "No." + String.valueOf(System.currentTimeMillis());
		Orders order = new Orders(orderNo, (Long) session.getAttribute("userId"), commodityId, price, new Date());
		ordersDao.saveOrUpdate(order);

		Commodity commodity = adminService.getCommodity(commodityId);
		commodity.setIsOrdered(true);
		commodityDao.updateStatus(commodity);

		ordersDao.getOrder(orderNo);
		map.put("orderId", order.getOrderId());
		System.out.println("orderId:" + order.getOrderId());
		
		map.put("msg", "操作成功！！！");
		map.put("msg1", "返回首页");
		map.put("adr", "home");
		map.put("msg2", "查看我的订单");
		map.put("adr1", "myOrders?curPage=1");
		
		return "success";
	}

	// 个人资料页面
	@RequestMapping(value = "/userInfo")
	public String userInfo(HttpSession session, Map<String, Object> map) {
		User user = adminService.getUser((Long) session.getAttribute("userId"));
		map.put("user", user);
		return "userInfo";
	}

	// 个人资料修改成功页面
	@RequestMapping(value = "/successU", method = RequestMethod.POST)
	public String userInfo(User user, Map<String, Object> map) {
		userDao.saveOrUpdate(user);
		map.put("msg", "修改成功！！！");
		map.put("msg1", "返回首页");
		map.put("adr", "home");
		return "success";
	}

	// 我的订单
	@RequestMapping("/myOrders")
	public String myOrders(@RequestParam(value = "curPage", required = false) Integer curPage, HttpSession session,
			Map<String, Object> map) {
		if (curPage != null)
			page.setCurrentPage(curPage);
		Long userId = (Long) session.getAttribute("userId");
		page.setTotalNum(userDao.numOfOrders(userId).intValue());
		List<Orders> orders = userDao.getOrders(userId, page);
		map.put("orders", orders);
		map.put("page", page);

		Map<Long, String> sellerMap = new HashMap<Long, String>();
		Map<Long, String> cMap = new HashMap<Long, String>();
		for (Orders o : orders) {
			sellerMap.put(o.getCommodityId(), adminService.getSellerName(o.getCommodityId()));
			cMap.put(o.getCommodityId(), adminService.getCommodityName(o.getCommodityId()));
		}
		map.put("sellerMap", sellerMap);
		map.put("cMap", cMap);

		return "myOrders";
	}

	@RequestMapping("/myOrders/{id}")
	public String deleteMyOrders(@PathVariable("id") Long orderId) {
		Orders order = adminService.getOrder(orderId);
		if (order.getIsOver() == false) {
			Commodity c = adminService.getCommodity(order.getCommodityId());
			c.setIsOrdered(false);
			commodityDao.updateStatus(c);
		}

		ordersDao.deleteOrder(orderId);
		return "redirect:/myOrders?curPage=1";
	}

	@RequestMapping("/myOrders/pay")
	public String payMyOrder(@RequestParam("Id") Long orderId) {
		Orders o = adminService.getOrder(orderId);
		o.setIsPay(true);
		ordersDao.saveOrUpdate(o);
		return "redirect:/myOrders";
	}

	@RequestMapping("/myOrders/over")
	public String overMyOrder(@RequestParam("Id") Long orderId) {
		Orders o = adminService.getOrder(orderId);
		o.setIsOver(true);
		ordersDao.saveOrUpdate(o);
		return "redirect:/myOrders";
	}

	@RequestMapping("/myMessages")
	public String myMessages(@RequestParam(value = "curPage", required = false) Integer curPage, HttpSession session,
			Map<String, Object> map) {
		if (curPage != null)
			page.setCurrentPage(curPage);
		Long userId = (Long) session.getAttribute("userId");

		page.setTotalNum(userDao.numOfMessages(userId).intValue());
		List<Message> messages = userDao.getMessages(userId, page);
		map.put("page", page);
		map.put("messages", messages);

		Map<Long, String> cMap = new HashMap<Long, String>();
		for (Message m : messages) {
			cMap.put(m.getCommodityId(), adminService.getCommodityName(m.getCommodityId()));
		}
		map.put("cMap", cMap);

		return "myMessages";
	}

	@RequestMapping("/myMessages/{id}")
	public String deleteMyMessages(@PathVariable("id") Long messageId) {
		messageDao.deleteMessage(messageId);
		return "redirect:/myMessages?curPage=1";
	}

}
