package com.sht.dao.impl;

import java.util.List;

import org.hibernate.Session;
import org.hibernate.SessionFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Repository;
import org.springframework.transaction.annotation.Transactional;

import com.sht.dao.CommodityClassDao;
import com.sht.entity.Commodity;
import com.sht.entity.CommodityClass;
import com.sht.entity.Page;

@Repository
@Transactional
public class CommodityClassDaoImpl implements CommodityClassDao {

	@Autowired
	private SessionFactory sessionFactory;

	private Session getSession() {
		return sessionFactory.getCurrentSession();
	}

	@Override
	public void saveOrUpdate(CommodityClass commodityClass) {
		if (commodityClass.getCommodityClassId() == null) {
			getSession().save(commodityClass);
		} else {
			getSession().createQuery(
					"UPDATE CommodityClass cc SET cc.commodityClassName = :name WHERE cc.commodityClassId = :Id")
					.setParameter("name", commodityClass.getCommodityClassName())
					.setParameter("Id", commodityClass.getCommodityClassId()).executeUpdate();
		}

	}

	@Override
	public String getCommodityClassName(Long commodityClassId) {
		String name = (String) getSession()
				.createQuery("SELECT c.commodityClassName FROM CommodityClass c WHERE c.commodityClassId = :Id")
				.setParameter("Id", commodityClassId).uniqueResult();
		return name;
	}

	@Override
	public void deleteCommodityClass(Long commodityClassId) {
		getSession().createQuery("DELETE FROM CommodityClass cc WHERE cc.commodityClassId = :Id")
				.setParameter("Id", commodityClassId).executeUpdate();
	}

	@Override
	public List<CommodityClass> getCommodityClasses() {
		List<CommodityClass> commodityclasses = getSession().createQuery("FROM CommodityClass", CommodityClass.class)
				.list();
		return commodityclasses;
	}

	@Override
	public List<Commodity> getCommodities(Long commodityClassId, Page page) {
		List<Commodity> commodities = getSession()
				.createQuery("FROM Commodity c WHERE c.commodityClassId = :Id", Commodity.class)
				.setFirstResult((page.getCurrentPage() - 1) * page.getPageSize()).setMaxResults(page.getPageSize())
				.setParameter("Id", commodityClassId).list();
		return commodities;
	}

	@Override
	public Long numOfSingleCommodities(Long commodityClassId) {
		Long num = (Long) getSession().createQuery("SELECT COUNT(*) FROM Commodity c WHERE c.commodityClassId = :Id")
				.setParameter("Id", commodityClassId).uniqueResult();
		return num;
	}

	@Override
	public Long numOfcommodityClasses() {
		Long num = (Long) getSession().createQuery("SELECT COUNT(*) FROM CommodityClass").uniqueResult();
		return num;
	}

}
