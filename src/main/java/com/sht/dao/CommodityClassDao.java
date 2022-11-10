package com.sht.dao;

import java.util.List;

import com.sht.entity.Commodity;
import com.sht.entity.CommodityClass;
import com.sht.entity.Page;

public interface CommodityClassDao {

	public void saveOrUpdate(CommodityClass commodityClass);
	
	public Long numOfcommodityClasses();
	
	// 商品类名
	public String getCommodityClassName(Long commodityClassId);
	
	public void deleteCommodityClass(Long commodityClassId);

	// 商品类别信息
	public List<CommodityClass> getCommodityClasses();

	// 各类商品信息
	public List<Commodity> getCommodities(Long commodityClassId, Page page);

	// 单类商品数量
	public Long numOfSingleCommodities(Long commodityClassId);
}
