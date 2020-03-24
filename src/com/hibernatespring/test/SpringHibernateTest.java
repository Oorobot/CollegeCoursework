package com.hibernatespring.test;

import javax.sql.DataSource;

import java.sql.SQLException;

import org.junit.jupiter.api.Test;
import org.springframework.context.ApplicationContext;
import org.springframework.context.support.ClassPathXmlApplicationContext;

import com.sht.service.AdminService;


class SpringHibernateTest {

	private ApplicationContext ctx = null;
	private AdminService adminService = null;

	{
		ctx = new ClassPathXmlApplicationContext("applicationContext.xml");
		adminService = ctx.getBean(AdminService.class);
	}
	
	//���������ݿ�����״��
	@Test
	public void testDataSource() throws SQLException {
		DataSource dataSource = ctx.getBean(DataSource.class);
		System.out.println(dataSource.getConnection());
		adminService.getClass();
	}

}
