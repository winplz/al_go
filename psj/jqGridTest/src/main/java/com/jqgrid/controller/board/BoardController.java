package com.jqgrid.controller.board;


import javax.inject.Inject;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.ModelAttribute;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.servlet.mvc.support.RedirectAttributes;
import com.jqgrid.controller.main.MainController;
import com.jqgrid.dto.board.BoardDTO;
import com.jqgrid.service.board.BoardService;

@Controller
@RequestMapping("/board/*")
public class BoardController {
	private static final Logger logger=LoggerFactory.getLogger(MainController.class);
	
	@Inject
	private BoardService boardService;
	
	@RequestMapping(value="/register",method=RequestMethod.GET)
	public void main() throws Exception {
		
		logger.info("==========register==========");
		
	}
	
	@RequestMapping(value="/register",method=RequestMethod.POST)
	public String registPost(BoardDTO boardDTO,RedirectAttributes rttr)throws Exception{
		
		boardService.create(boardDTO);
		logger.info("====================글쓰기 완료====================");
		return "redirect:/main";
	}
	@RequestMapping(value="/readPage",method=RequestMethod.GET)
	public void readPageGET(@ModelAttribute("bno") Integer bno,
			Model model)throws Exception{
	
		model.addAttribute(boardService.getThisPage(bno));
		boardService.updateViewCount(bno);
		logger.info("====================상세 글보기====================");
	}
	@RequestMapping(value="/modifyPage",method=RequestMethod.GET)
	public void modifyPageGET(@ModelAttribute("bno") Integer bno,
			Model model)throws Exception{
	
		model.addAttribute(boardService.getThisPage(bno));
		logger.info("====================상세 수정 페이지====================");
	}
	@RequestMapping(value="/modifyPage",method=RequestMethod.POST)
	public String modifyPagingPOST(BoardDTO boardDTO,RedirectAttributes rttr) throws Exception{

		boardService.update(boardDTO);

		rttr.addFlashAttribute("msg","modify success");
		
		logger.info(rttr.toString());
		
		return "redirect:/main";
	}
	  @RequestMapping(value = "/removePage", method = RequestMethod.POST)
	  public String remove(@RequestParam("bno") int bno, RedirectAttributes rttr) throws Exception {
		
		boardService.delete(bno);
	    rttr.addFlashAttribute("msg", "remove success");

	    return "redirect:/main";
	  }
}
