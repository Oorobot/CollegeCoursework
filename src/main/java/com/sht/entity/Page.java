package com.sht.entity;

import org.springframework.stereotype.Component;

@Component
public class Page {

	// ҳ��С
	private int pageSize = 5;
	// ��ҳ��
	private int totalPage;
	// ��ǰҳ��
	private int currentPage = 1;
	// ��һҳ
	private int nextPage;
	// ��һҳ
	private int prePage;
	// �ܼ�¼����
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
