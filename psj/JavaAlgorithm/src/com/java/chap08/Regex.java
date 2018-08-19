package com.java.chap08;

import java.util.Scanner;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class Regex {

	public static void main(String[] args) {
		Scanner stdIn = new Scanner(System.in);

		//System.out.print("텍스트：");
		String s1 ="asdf s23#$@#4 Disk overload(/APWAS2,vnx5800:/APWAS2):96%(1 times) in 60 second"; // Test용 문자열"

		System.out.print("패턴：");
		String s2 = stdIn.next(); // 패턴용 문자열 Disk+.*second
		Pattern pattern = Pattern.compile(s2);
		Matcher matcher = pattern.matcher(s1);
		System.out.println();
		int count = 0;

		// 정규 표현에 검색된 문자열 구하기
		// find() 메소드가 false 반환할 때까지 반복
		while (matcher.find()) {
			// group() 메소드를 호출하고 정규 표현에 일치된 문자열을 꺼냄
			count++;
			System.out.print(matcher.group());
		}
		System.out.println("\n");
		System.out.println(matcher.toMatchResult());
		System.out.println("\n정규표현식으로 찾은 문자의 개수=" + count + "개");
	}
}
