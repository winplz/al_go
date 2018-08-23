package com.java.chap01;

import java.util.Scanner;

public class Q11 {

	public static void main(String[] args) {
		// TODO Auto-generated method stub

		Scanner sc=new Scanner(System.in);
		
		System.out.println("숫자를 입력하세요 : ");
		int n=sc.nextInt();
		int a=0;
		
		while(n>0) {
			n=n/10;
			a++;
		}
		System.out.println("해당 숫자의 자리수 : " + a);
	}

}
