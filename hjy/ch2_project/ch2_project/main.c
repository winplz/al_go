//
//  main.c
//  ch2_project
//
//  Created by winplz on 2018. 8. 22..
//  Copyright © 2018년 winplz. All rights reserved.
//

#include <iostream>


int minof(const int a[], int n);
int sumof(const int a[], int n);
double aveof(const int a[], int n);
int card_conv(unsigned x, int n, char d[]);


int main(int argc, const char * argv[]) {
    // insert code here...
    
    return 0;
}

string solution(int a, int b) {
    // 2016 01 01 FRI, leap year
    string day[] = {"SUN","MON","TUE","WED","THU","FRI","SAT"};
    int nArrMonth[] = {31,29,31,30,31,30,31,31,30,31,30,31};
    int nCount = 5; // 2016 1 1 FRI
    for(int i=0; i<a; i++)
    {
        for(int j=1; j<=nArrMonth[i]; j++)
        {
            if(i==a-1 && j==b)
            {
                break;
            }
            
            nCount++;
            if(nCount == 7)
                nCount = 0;
            
        }
    }
    string answer = day[nCount];
    return answer;
}
