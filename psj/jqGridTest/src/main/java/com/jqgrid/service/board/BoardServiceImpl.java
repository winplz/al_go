package com.jqgrid.service.board;

import java.util.List;

import javax.inject.Inject;

import org.springframework.stereotype.Service;

import com.jqgrid.dao.board.BoardDAO;
import com.jqgrid.dto.board.BoardDTO;
import com.jqgrid.util.page.JsonObj;

@Service
public class BoardServiceImpl implements BoardService {

	@Inject
	private BoardDAO boardDAO;
	
	@Override
	public void create(BoardDTO boardDTO) throws Exception {
		// TODO Auto-generated method stub
		boardDAO.create(boardDTO);
	}
	@Override
	public List<BoardDTO> getAllContents(String page, String rows) throws Exception {
		// TODO Auto-generated method stub
		return boardDAO.getAllContents(page, rows);
	}
	@Override
	public JsonObj getCount() throws Exception {
		// TODO Auto-generated method stub
		return boardDAO.getCount();
	}
	@Override
	public BoardDTO getThisPage(Integer bno) throws Exception {
		// TODO Auto-generated method stub
		return boardDAO.getThisPage(bno);
	}
	@Override
	public void update(BoardDTO boardDTO) throws Exception {
		// TODO Auto-generated method stub
		boardDAO.update(boardDTO);
	}
	@Override
	public void delete(Integer bno) throws Exception {
		// TODO Auto-generated method stub
		boardDAO.delete(bno);
	}
	@Override
	public void updateViewCount(Integer bno) throws Exception {
		// TODO Auto-generated method stub
		boardDAO.updateViewCount(bno);
	}
	@Override
	public void boardNumReset() throws Exception {
		// TODO Auto-generated method stub
		boardDAO.boardNumReset();
	}
}
