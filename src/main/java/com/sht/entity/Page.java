package com.sht.entity;

import org.springframework.stereotype.Component;

@Component
public class Page {

	// 页大小
	private int pageSize = 5;
	// 总页数
	private int totalPage;
	// 当前页号
	private int currentPage = 1;
	// 下一页
	private int nextPage;
	// 上一页
	private int prePage;
	// 总记录条数
	private int totalNum;

	public int getPageSize() {
		return pageSize;
	}

	public void setPageSize(int pageSize) {
		this.pageSize = pageSize;
	}

	public int gettotalPage() {
		totalPage = totalNum % pageSize == 0 ? totalNum / pageSize : totalNum / pageSize + 1;
		return totalPage;
	}

	public void settotalPage(int totalPage) {
		this.totalPage = totalPage;
	}

	public int getCurrentPage() {
		return currentPage;
	}

	public void setCurrentPage(int currentPage) {
		this.currentPage = currentPage;
	}

	public int getNextPage() {
		if (currentPage < totalPage)
			nextPage = currentPage + 1;
		else
			nextPage = currentPage;
		return nextPage;
	}

	public void setNextPage(int nextPage) {
		this.nextPage = nextPage;
	}

	public int getTotalNum() {
		return totalNum;
	}

	public void setTotalNum(int totalNum) {
		this.totalNum = totalNum;
	}

	public int getPrePage() {
		if (currentPage > 1)
			prePage = currentPage - 1;
		else
			prePage = currentPage;
		return prePage;
	}

	public void setPrePage(int prePage) {
		this.prePage = prePage;
	}

	public Page(int pageSize, int totalNum) {
		super();
		this.pageSize = pageSize;
		this.totalNum = totalNum;
	}

	public Page(int pageSize, int totalPage, int currentPage, int nextPage, int prePage, int totalNum) {
		super();
		this.pageSize = pageSize;
		this.totalPage = totalPage;
		this.currentPage = currentPage;
		this.nextPage = nextPage;
		this.prePage = prePage;
		this.totalNum = totalNum;
	}

	public Page() {
	}

}
