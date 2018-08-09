package com.jqgrid.dao.user;

import javax.inject.Inject;

import org.apache.ibatis.session.SqlSession;
import org.springframework.stereotype.Repository;
import com.jqgrid.dto.user.UserDTO;

@Repository
public class UserDAOImpl implements UserDAO {

	@Inject
	private SqlSession session;
	
	@Override
	public void register(UserDTO userDTO) throws Exception {
		// TODO Auto-generated method stub
		session.insert("user.register",userDTO);
	}
}
