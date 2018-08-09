package com.jqgrid.util.security;

import java.io.IOException;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.security.core.Authentication;
import org.springframework.security.core.GrantedAuthority;
import org.springframework.security.core.userdetails.UserDetails;
import org.springframework.security.web.authentication.AuthenticationSuccessHandler;
import org.springframework.security.web.authentication.SavedRequestAwareAuthenticationSuccessHandler;

public class UserLoginSuccessHandler extends SavedRequestAwareAuthenticationSuccessHandler implements AuthenticationSuccessHandler {

	private static final Logger logger = LoggerFactory.getLogger(UserLoginSuccessHandler.class);

	@Override
	public void onAuthenticationSuccess(HttpServletRequest req, HttpServletResponse res, Authentication auth)
			throws IOException, ServletException {
		// TODO Auto-generated method stub
		logger.info("유저 아이디 : {}",auth.getName());
		logger.info("유저 권한: {}",auth.getAuthorities().toString());
		logger.info("유저 Details : {}",auth.getDetails().toString());
		logger.info("유저 Principal : {}",auth.getPrincipal().toString());
		for (GrantedAuthority a : auth.getAuthorities()) {
			logger.info(a.getAuthority());
		}

		UserDetails u = (UserDetails) auth.getPrincipal();

		logger.info(String.valueOf(u.isAccountNonExpired()));
		logger.info(String.valueOf(u.isAccountNonLocked()));
		logger.info(String.valueOf(u.isCredentialsNonExpired()));
		logger.info(String.valueOf(u.isEnabled()));

		HttpSession session = req.getSession();
		if(session != null) {
			String path = (String) session.getAttribute("prevpage");
	
/*			전페이지가 회원가입 페이지일경우 메인으로바꿈
			if(path.contains("main")) {
				path = path.replace("user/signup", "main");	
			}
			로그인 버튼 두번 클릭시 로그인 후 다시 로그인 페이지로 가는거 방지(로그인->메인)
			else if(path.contains("login")) {
				path = path.replace("user/login", "main");	
			}*/
			if(path != null) {
				session.removeAttribute("prevpage");
				getRedirectStrategy().sendRedirect(req, res, path);
			}else {
				super.onAuthenticationSuccess(req, res, auth);
			}
		} else {
			super.onAuthenticationSuccess(req, res, auth);
		}
	}

}