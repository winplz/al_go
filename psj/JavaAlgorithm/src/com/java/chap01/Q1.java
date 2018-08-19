package com.java.chap01;

public class Q1 {
	private static int max4(int a, int b, int c, int d) {
		int max = a;
	
		if (max >= b)
			if (max >= c)
				if (max >= d)
					//max = a;
		if (b >= max)
			if (b >= c)
				if (b >= d)
					max = b;
		if (c >= max)
			if (c >= b)
				if (c >= d)
					max = c;
		if (d >= max)
			if (d >= b)
				if (d >= c)
					max = d;
			
		System.out.println(max);
		return max;
	}

	public static void main(String[] args) {
		// TODO Auto-generated method stub
		
		max4(10, 9, 0, 3);
		max4(1330, 49, 20, 13);
		max4(120, 449, 123, 23);
		max4(1110, 339, 11110, 13);
	}

}
