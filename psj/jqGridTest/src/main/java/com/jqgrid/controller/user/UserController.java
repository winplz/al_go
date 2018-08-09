package com.jqgrid.controller.user;

import javax.inject.Inject;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.servlet.mvc.support.RedirectAttributes;
import com.jqgrid.dto.user.UserDTO;
import com.jqgrid.service.user.UserService;
import com.jqgrid.util.security.ShaEncoder;

@Controller
@RequestMapping("/user/*")
public class UserController {

	private static final Logger logger=LoggerFactory.getLogger(UserController.class);
	
	@Inject
	private UserService userService;
	
	@Inject
	private ShaEncoder shaEncoder;
	
	@RequestMapping(value="/login", method=RequestMethod.GET)
	public void login() {
		logger.info("====================로그인 페이지====================");
	}
	@RequestMapping(value="/signup", method=RequestMethod.GET)
	public void signUp() {
		logger.info("====================회원가입 페이지====================");
	}
	@RequestMapping(value="/signupSuccess",method=RequestMethod.POST)
	public String signUpsuccessPost(UserDTO userDTO,RedirectAttributes rttr) throws Exception {

		userDTO.setMpwd(shaEncoder.saltEncoding(userDTO.getMpwd(),userDTO.getMid()));
		userService.register(userDTO);
		rttr.addFlashAttribute("msg","usersignup success");
		logger.info("====================사용자 정보===================="
		+"\n"+userDTO);
		logger.info("====================회원가입 성공====================");
		return "redirect:/main";
	}
}
