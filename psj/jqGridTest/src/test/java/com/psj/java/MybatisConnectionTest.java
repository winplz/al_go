package com.psj.java;

import java.sql.Connection;

import javax.inject.Inject;
import javax.sql.DataSource;

import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.test.context.ContextConfiguration;
import org.springframework.test.context.junit4.SpringJUnit4ClassRunner;

@RunWith(SpringJUnit4ClassRunner.class)
@ContextConfiguration(locations="file:src\\main\\webapp\\WEB-INF\\spring\\root-context.xml")
public class MybatisConnectionTest {

	@Inject
	private DataSource dataSource;
	
	@Test
	public void connectionTest()throws Exception {
		try(Connection conn=dataSource.getConnection()) {
			System.out.println("MySQL 연결 성공=>"+conn);
		}catch(Exception e) {
			e.printStackTrace();
		}
	}
}
