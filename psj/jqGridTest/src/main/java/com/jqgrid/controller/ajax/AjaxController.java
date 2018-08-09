package com.jqgrid.controller.ajax;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import javax.inject.Inject;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.ResponseBody;
import org.springframework.web.bind.annotation.RestController;
import com.jqgrid.dto.board.BoardDTO;
import com.jqgrid.service.board.BoardService;
import com.jqgrid.util.page.JsonObj;

@RestController
@RequestMapping("/contents")
public class AjaxController {

	@Inject 
	private BoardService boardSerivce;
	
	private static final Logger logger=LoggerFactory.getLogger(AjaxController.class);
	
	@RequestMapping(value="/getList",method=RequestMethod.GET)
	@ResponseBody
	public JsonObj getJqGridJson(
			@RequestParam(value="page", required=false, defaultValue="1") String page,
			@RequestParam(value="rows", required=false, defaultValue="") String rows)throws Exception{
		

		// dao에서 리스트 형식으로된 데이터를 DB에서 가져옴.	 

		logger.info("====================리스트 호출===================="+page);
		List<BoardDTO> list = boardSerivce.getAllContents(page, rows);
		System.out.println(list);
		List<Map<String, Object>> resultList = new ArrayList<Map<String, Object>>();

		JsonObj totalCnt = boardSerivce.getCount();

		HashMap<String, Object> tempMap = new HashMap<String, Object>();
		System.out.println(page);
		// list 형식으로 된 데이터의 크기를 구함.
		int size = list.size();
		System.out.println(size);


		for(int i=0; i<size; i++){

			tempMap.put("bno", list.get(i).getBno());
			tempMap.put("title", list.get(i).getTitle());
			tempMap.put("content", list.get(i).getContent());
			tempMap.put("writer", list.get(i).getWriter());
			tempMap.put("regdate", list.get(i).getRegdate());
			tempMap.put("viewcnt", list.get(i).getViewcnt());
			
			resultList.add(tempMap);
			
			tempMap = new HashMap<String, Object>();
		}

		JsonObj jsonObj = new JsonObj();
		
		//jsonObj.setRecords(totalCnt.getRecords());
		//jsonObj.setTotal(totalCnt.getTotal());
		//jsonObj.setPage(10);
		jsonObj.setRows(resultList);
		
		return jsonObj;
	}
}
