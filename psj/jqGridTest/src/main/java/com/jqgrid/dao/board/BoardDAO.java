package com.jqgrid.dao.board;

import java.util.List;

import com.jqgrid.dto.board.BoardDTO;
import com.jqgrid.util.page.JsonObj;

public interface BoardDAO {

	public void create(BoardDTO boardDTO)throws Exception;
	public List<BoardDTO> getAllContents(String page,String rows)throws Exception;
	public JsonObj getCount()throws Exception;
	public BoardDTO getThisPage(Integer bno)throws Exception;
	public void update(BoardDTO boardDTO)throws Exception;
	public void delete(Integer bno)throws Exception;
	public void updateViewCount(Integer bno)throws Exception;
	public void boardNumReset()throws Exception;
}
