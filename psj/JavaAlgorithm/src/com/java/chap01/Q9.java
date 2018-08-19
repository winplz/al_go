package com.java.chap01;

public class Q9 {

	static int sumof(int a, int b) {
		int sum = 0;
		for (int i = a; i <= b; i++) {
			System.out.println(i);
			sum += i;
		}
		return sum;
	}

	public static void main(String args[]) {
		int result = sumof(5, 8);
		System.out.println("결과값 : " + result);
	}
}
