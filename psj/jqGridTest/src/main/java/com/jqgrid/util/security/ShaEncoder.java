package com.jqgrid.util.security;

import javax.annotation.Resource;

import org.springframework.security.authentication.encoding.ShaPasswordEncoder;
import org.springframework.security.crypto.password.PasswordEncoder;
import org.springframework.stereotype.Service;

@Service("shaEncoder")
public class ShaEncoder implements PasswordEncoder{
	
	@Resource(name="passwordEncoder")
	private ShaPasswordEncoder encoder;
	private Object salt = null;
	public String encoding(String str){
		return encoder.encodePassword(str,null);
	}

	public String saltEncoding(String str,String salt){
		return encoder.encodePassword(str,salt);
	}
	@Override public String encode(CharSequence rawPassword) { 
		return encoder.encodePassword(rawPassword.toString(), salt); 
		} 
	
	@Override public boolean matches(CharSequence rawPassword, String encodedPassword) { 
		return encoder.isPasswordValid(encodedPassword, rawPassword.toString(), salt); 
		}

}
