package com.jqgrid.util.page;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;

public class JsonObj {

	private int page;
	private int records;
	private int total;
	private List<Map<String, Object>> rows = new ArrayList<Map<String, Object>>();

	public int getPage() {
		return page;
	}
	public void setPage(int page) {
		this.page = page;
	}
	public int getRecords() {
		return records;
	}
	public void setRecords(int records) {
		this.records = records;
	}
	public int getTotal() {
		return total;
	}
	public void setTotal(int total) {
		this.total = total;
	}
	public List<Map<String, Object>> getRows() {
		return rows;
	}
	public void setRows(List<Map<String, Object>> rows) {
		this.rows = rows;
	}
	@Override
	public String toString() {
		return "JsonObj [page=" + page + ", records=" + records + ", total=" + total + ", rows=" + rows + "]";
	}
	
	
}