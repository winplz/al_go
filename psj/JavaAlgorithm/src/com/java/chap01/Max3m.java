package com.java.chap01;

import java.util.Scanner;

public class Max3m {
/*	static int max3(int a, int b, int c) {
		int max=a;
		if(b>max) {
			max=b;
		}
		if(c>max)	{
			max=c;
		}
		return max;
	}*/
	public static void main(String[] args) {
		/*System.out.println(max3(3,2,1));
		System.out.println(max3(3,2,2));
		System.out.println(""+max3(3,1,2));
		System.out.println(""+max3(3,2,3));
		System.out.println(""+max3(2,1,3));
		System.out.println(""+max3(3,3,3));
		System.out.println(""+max3(2,2,3));
		System.out.println(""+max3(2,3,1));*/
		
		Scanner sc=new Scanner(System.in);
		
		System.out.println("a값입력:");
		int a =sc.nextInt();
		System.out.println("b값입력:");
		int b=sc.nextInt();
		System.out.println("c값입력:");
		int c=sc.nextInt();
		
		int min=a;
		
		if(b<min) {
			min=b;
		}
		if(c<min) {
			min=c;
		}
		
		System.out.println("min="+min);
		
		
		
	}
}
