package com.java.chap01;

import java.util.Scanner;

public class Q8 {

	public static void main(String[] args) {
		// TODO Auto-generated method stub
		Scanner sc = new Scanner(System.in);

		System.out.println("=====가우스의 덧셈=====");
		System.out.println("n값 입력:");
		int n = sc.nextInt();

		// 가우스 덧셈의 공식
		int sum = (1 + n) * n / 2;

		System.out.println("합계:" + sum);
	}

}
