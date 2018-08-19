package com.java.chap08;

import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;
// Boyer-Moore법으로 문자열을 검색 
import java.util.regex.Matcher;
import java.util.regex.Pattern;

class BMmatch {
	// Boyer-Moore법으로 문자열을 검색
	static int bmMatch(String txt, String pat) {
		int pt; // txt 커서
		int pp; // pat 커서
		int txtLen = txt.length(); // txt의 문자 개수
		int patLen = pat.length(); // pat의 문자 개수
		int[] skip = new int[Character.MAX_VALUE + 1]; // 건너뛰기 표

		// 건너뛰기 표 만들기
		for (pt = 0; pt <= Character.MAX_VALUE; pt++)
			skip[pt] = patLen;
		for (pt = 0; pt < patLen - 1; pt++)
			skip[pat.charAt(pt)] = patLen - pt - 1; // pt == patLen - 1
		// 검색
		while (pt < txtLen) {
			pp = patLen - 1; // pat의 끝 문자에 주목

			while (txt.charAt(pt) == pat.charAt(pp)) {
				if (pp == 0)
					return pt; // 검색 성공
				pp--;
				pt--;
			}
			pt += (skip[txt.charAt(pt)] > patLen - pp) ? skip[txt.charAt(pt)] : patLen - pp;
		}
		return -1; // 검색 실패
	}

	public static void main(String[] args) {
		Scanner stdIn = new Scanner(System.in);

		//System.out.println("텍스트：");
		String s1 = "asdf s23#$@#4 Disk overload(/APWAS2,vnx5800:/APWAS2):96%(1 times) in 60 second"; // 텍스트용 문자열

		System.out.print("패턴：");
		String s3 = stdIn.nextLine(); // 패턴용 문자열
		
		Pattern pattern = Pattern.compile(s3);
		Matcher matcher = pattern.matcher(s1);
		
		List<String> list=new ArrayList<String>();
		
		while(matcher.find()) {
			String result=matcher.group(0);
			System.out.println("결과값="+result);
			list.add(result);
		}
		s3=list.toString();
		System.out.println(s3);
		int idx = bmMatch(s1, s3);

		
		if (idx == -1) {
			System.out.println("텍스트에 패턴이 없습니다.");
		} else {
			int len = 0;
			for (int i = 0; i < idx; i++) {
				len += s1.substring(i, i + 1).getBytes().length;
			}
			len += s3.length();

			System.out.println((idx + 1) + "번째 문자와 일치합니다.");
			System.out.println("텍스트：" + s1);
			System.out.printf(String.format("패턴：%%%ds\n", len), s3);
		}

	}
}
