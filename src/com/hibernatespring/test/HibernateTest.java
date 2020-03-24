package com.hibernatespring.test;

import java.util.List;

import javax.persistence.criteria.CriteriaBuilder;
import javax.persistence.criteria.CriteriaQuery;
import javax.persistence.criteria.Predicate;
import javax.persistence.criteria.Root;

import org.hibernate.Session;
import org.hibernate.SessionFactory;
import org.hibernate.Transaction;
import org.hibernate.cfg.Configuration;
import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import com.sht.entity.Commodity;
import com.sht.entity.Message;
import com.sht.entity.Page;
import com.sht.entity.User;

class HibernateTest {

	private SessionFactory sessionFactory;
	private Session session;
	private Transaction transaction;

	@BeforeEach
	public void Init() {
		System.out.println("init...");
		Configuration configuration = new Configuration().configure("hibernate.cfg.xml");
		sessionFactory = configuration.buildSessionFactory();

		session = sessionFactory.openSession();
		transaction = session.beginTransaction();
	}

	@AfterEach
	public void destory() {
		System.out.println("destory...");
		transaction.commit();
		session.close();
		sessionFactory.close();
	}

	@Test
	void testout() {
		System.out.println("out..");
	}

	@Test
	public void TestCriteria() {
		CriteriaBuilder builder = session.getCriteriaBuilder();
		CriteriaQuery<User> criteriaQuery = builder.createQuery(User.class);

//		�趨 �� ʵ��
		Root<User> root = criteriaQuery.from(User.class);
		// ѡ���ѯ������
		criteriaQuery.select(root);
		// ��ѯ������
		Predicate username = builder.equal(root.get("username"), "17122962");
		// ��ѯ
		criteriaQuery.where(username);
		// ���
		List<User> users = session.createQuery(criteriaQuery).getResultList();
		// ��ӡ
		for (User u : users) {
			System.out.println(u);
		}
	}

	// com.sht.dao.impl ʵ�ֲ���
	@Test
	public void TestUser() {
		/*
		 * User user = new User("17120011", "abcd11"); session.save(user);
		 */

		/*
		 * session.createQuery("UPDATE User u SET u.name = :name" +
		 * ", u.phone = :phone " + ", u.sex = :sex" + ", u.address=:address " +
		 * "WHERE u.userId = :Id ").setParameter("name", "Mike") .setParameter("phone",
		 * "18018881999").setParameter("sex", true) .setParameter("address",
		 * "�����ʹ�188��¥199�ŷ�").setParameter("Id", (long) 5).executeUpdate();
		 */

		User user = (User) session.createQuery("FROM User u WHERE u.userId = :Id").setParameter("Id", (long) 5)
				.uniqueResult();
		System.out.println(user);

		Long count = (Long) session.createQuery("SELECT COUNT(*) FROM User u WHERE u.username = :un")
				.setParameter("un", "1712962").uniqueResult();
		System.out.println(count);

		Long Id = (Long) session.createQuery("SELECT u.userId FROM User u WHERE u.username = :un and u.password = :pw")
				.setParameter("un", "17120000").setParameter("pw", "0000").uniqueResult();
		System.out.println(Id);
	}

	@Test
	void TestCommodity() {

		Long count = (Long) session.createQuery("SELECT COUNT(*) FROM Commodity").uniqueResult();
		System.out.println(count);

		Page page = new Page(2, 3);

		List<Commodity> Cs = session.createQuery("FROM Commodity", Commodity.class)
				.setFirstResult((page.getCurrentPage() - 1) * page.getPageSize()).setMaxResults(page.getPageSize())
				.list();
		System.out.println(Cs);

		String string = "����";
		List<Commodity> Cs1 = session
				.createQuery("SELECT c FROM Commodity c WHERE c.commodityName LIKE :cn", Commodity.class)
				.setParameter("cn", "%" + string + "%").list();
		System.out.println(Cs1);

		session.createQuery("UPDATE Commodity c SET c.isOrdered = :Is WHERE c.commodityId = :Id")
				.setParameter("Is", false).setParameter("Id", (long) 1).executeUpdate();

		session.createQuery("DELETE Commodity c WHERE c.commodityId = :Id").setParameter("Id", (long) 6)
				.executeUpdate();

		List<Message> Ms = session.createQuery("FROM Message m WHERE m.commodityId = :Id", Message.class)
				.setParameter("Id", (long) 1).list();
		System.out.println(Ms);
	}

}
