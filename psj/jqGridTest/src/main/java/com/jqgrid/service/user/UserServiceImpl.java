package com.jqgrid.service.user;

import javax.inject.Inject;
import org.springframework.stereotype.Service;
import com.jqgrid.dao.user.UserDAO;
import com.jqgrid.dto.user.UserDTO;

@Service
public class UserServiceImpl implements UserService {

	@Inject 
	private UserDAO userDAO;
	
	@Override
	public void register(UserDTO userDTO) throws Exception {
		// TODO Auto-generated method stub
		userDAO.register(userDTO);
	}

}
