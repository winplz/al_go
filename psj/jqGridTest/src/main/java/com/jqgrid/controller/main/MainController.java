package com.jqgrid.controller.main;

import javax.inject.Inject;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;

import com.jqgrid.service.board.BoardService;

@Controller
public class MainController {

	private static final Logger logger=LoggerFactory.getLogger(MainController.class);
	
	@Inject
	private BoardService boardService;
	
	@RequestMapping(value="/main",method=RequestMethod.GET)
	public void main() throws Exception {
		boardService.boardNumReset();
		logger.info("====================main 실행====================");
	}
	
}
