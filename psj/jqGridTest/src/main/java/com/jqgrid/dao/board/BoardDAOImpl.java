package com.jqgrid.dao.board;

import java.util.HashMap;
import java.util.List;
import javax.inject.Inject;
import org.apache.ibatis.session.SqlSession;
import org.springframework.stereotype.Repository;
import com.jqgrid.dto.board.BoardDTO;
import com.jqgrid.util.page.JsonObj;

@Repository
public class BoardDAOImpl implements BoardDAO {

	@Inject
	private SqlSession session;
	
	@Override
	public void create(BoardDTO boardDTO) throws Exception {
		// TODO Auto-generated method stub
		session.insert("board.create",boardDTO);
	}
	@Override
	public List<BoardDTO> getAllContents(String page, String rows) throws Exception {
		// TODO Auto-generated method stub
		HashMap <String,Object> map =new HashMap<String,Object>();
		map.put("page", page);
		map.put("rows",rows);
		return session.selectList("board.getList",map);
	}
	@Override
	public JsonObj getCount() throws Exception {
		// TODO Auto-generated method stub
		return session.selectOne("board.getCount");
	}
	@Override
	public BoardDTO getThisPage(Integer bno) throws Exception {
		// TODO Auto-generated method stub
		return session.selectOne("board.getThisPage",bno);
	}
	@Override
	public void update(BoardDTO boardDTO) throws Exception {
		// TODO Auto-generated method stub
		session.update("board.update",boardDTO);
	}
	@Override
	public void delete(Integer bno) throws Exception {
		// TODO Auto-generated method stub
		session.delete("board.delete",bno);
	}
	@Override
	public void updateViewCount(Integer bno) throws Exception {
		// TODO Auto-generated method stub
		session.update("board.viewCount",bno);
	}
	@Override
	public void boardNumReset() throws Exception {
		// TODO Auto-generated method stub
		session.update("board.boardNumReset");
	}
}
