package com.java.chap01;

import java.util.Scanner;

public class multiple1_12 {

	public static void main(String[] args) {
		// TODO Auto-generated method stub
		
		Scanner sc=new Scanner(System.in);
		System.out.println("단을 입력하세요:");
		int n=sc.nextInt();
		System.out.println("사가형을 출력합니다.");
		for(int i=1; i<= n; i++) {
			for(int j=1; j<=n; j++) {				
				System.out.print("*");
			}
			System.out.println();
		}
	}
}