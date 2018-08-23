package test;

import java.util.Scanner;

public class rcopy {
	public static void rcopy(int[] a, int[] b) {
		for (int i = 0; i <= b.length - 1; i++) {
			a[i]=b[i];
		}
	}

	public static void main(String[] args) {
		Scanner sc = new Scanner(System.in);

		System.out.println("a배열의 요소의 갯수:");
		int aCount = sc.nextInt();
		
		int[] a=new int[aCount];
		for (int i = 0; i < aCount; i++) {
			System.out.println("a의 배열값 입력:");
			a[i]=sc.nextInt();
		}
		System.out.println("b배열의 요소의 갯수:");
		int bCount = sc.nextInt();
		int[] b=new int[bCount];
		for (int j = 0; j < bCount; j++) {
			System.out.println("b의 배열값 입력:");
			b[j]=sc.nextInt();
		}

		System.out.println("rcopy() 호출");
		rcopy(a, b);
		
		for(int k=0; k<aCount; k++){
			System.out.println("b의 배열을 a의 배열에 복사한 결과:"+a[k]);
		}
	}
}
