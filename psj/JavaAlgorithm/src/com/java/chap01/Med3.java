package com.java.chap01;

import java.util.Scanner;

//중앙값 구하기
public class Med3 {

	static int med3(int a, int b, int c) {
		if ((b >= a && c <= a) || (b <= a && c >= a)) {
			return a;
		} else if ((a > b && c < b) || a < b && c > b) {
			return b;
		}
		return c;
	}

	public static void main(String[] args) {
		// TODO Auto-generated method stub
		Scanner sc=new Scanner(System.in);
		System.out.println("a값입력:"); int a=sc.nextInt();
		System.out.println("b값입력:"); int b=sc.nextInt();
		System.out.println("c값입력:"); int c=sc.nextInt();
		System.out.println("중앙값:"+med3(a,b,c));
	}

}
