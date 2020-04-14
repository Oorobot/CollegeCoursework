package com.sht.dao;

import java.util.List;

import com.sht.entity.Commodity;
import com.sht.entity.CommodityClass;
import com.sht.entity.Page;

public interface CommodityClassDao {

	public void saveOrUpdate(CommodityClass commodityClass);
	
	public Long numOfcommodityClasses();
	
	// ��Ʒ����
	public String getCommodityClassName(Long commodityClassId);
	
	public void deleteCommodityClass(Long commodityClassId);

	// ��Ʒ�����Ϣ
	public List<CommodityClass> getCommodityClasses();

	// ������Ʒ��Ϣ
	public List<Commodity> getCommodities(Long commodityClassId, Page page);

	// ������Ʒ����
	public Long numOfSingleCommodities(Long commodityClassId);
}
