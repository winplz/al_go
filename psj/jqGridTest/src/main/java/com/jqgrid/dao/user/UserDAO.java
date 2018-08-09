package com.jqgrid.dao.user;


import com.jqgrid.dto.user.UserDTO;

public interface UserDAO {

	public void register(UserDTO userDTO) throws Exception;
	
}
