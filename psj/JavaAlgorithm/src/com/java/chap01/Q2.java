package com.java.chap01;

public class Q2 {

	public static int min3(int a, int b, int c) {
		int min=0;
		if(a<=b)
			if(a<=c)
				min=a;

		if(b<=a)
			if(b<=c)
				min=b;

		if(c<=a)
			if(c<=b)
				min=c;
		System.out.println(min);
				return min;
	}
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		min3(1,2,3);
		min3(1444,22,33);
		min3(7741,222,3);
	}

}
