//
//  main.c
//  c_project
//
//  Created by winplz on 2018. 8. 15..
//  Copyright © 2018년 winplz. All rights reserved.
//

#include <stdio.h>

#define MAX 5
int max(int a, int b);
int min(int a, int b);
void exam_max_min();
void exam_for(int input);
void exam_print();
void exam_square();

void exam_triangle();
void exam_spira();
void exam_nrpira();


int main(int argc, const char * argv[]) {

    
    //exam_max_min();
    //exam_for(5);
    exam_print();
    //exam_square();
    //exam_spira();
    //exam_nrpira();
    
    return 0;
}


/*
int max(int a, int b,int c)
{
    int max = 0;
    // 삼항연산자 사용
    max = (a >= b) ? ((a>=c) ? a : c) : ((b>=c) ? b : c);
    

    //if~else 사용
    max = a;
    if(b >= max)
        max = b;
    if(c >= max)
        max = c;
 
    return max;
}
*/
int max(int a, int b)
{
    return (a >= b) ? a : b;
}
int min(int a, int b)
{
    return (a <= b) ? a : b;
}
void exam_max_min()
{
    int nArr[MAX] = {0,};
    int nReturn = 0;
    int nInput = 0;
    //memset(nArr,0,MAX);
    
    printf("Chapter 01 연습문제\n");
    printf("MAX Count : 5\n");
    printf("input value : 0, stop\n");
    
    for(int i=0; i<MAX; i++)
    {
        printf("input (%d) : ",i);
        scanf("%d",&nArr[i]);
        if(nArr[i] == 0)
            break;
    }
    printf("MAX : 0, MIN : 1\n");
    scanf("%d",&nInput);
    
    nReturn = nArr[0];
    for(int i=0; i<MAX-1; i++)
    {
        if(nArr[i+1] == 0)
            break;
        if(nInput != 1)
            nReturn = max(nReturn, nArr[i+1]);
        else
            nReturn = min(nReturn, nArr[i+1]);
    }
    
    for(int i=0; i<MAX-1; i++)
    {
        printf("%2d",nArr[i]);
        if(nArr[i+1] == 0)
            break;
    }
    
    printf("\nMAX value : %d\n",nReturn);

}

void exam_for(int input)
{
    int sum = 0;
    for(int i=1; i<=input; i++)
    {
        sum +=i;
    }
    printf("1~%d, sum : %d\n",input, sum);
}

void exam_print()
{
    printf("  | ");
    for(int i=1; i<=9; i++)
    {
        printf("%3d",i);
    }
    printf("\n--+------------------------------\n");
    for(int i=1; i<=9; i++)
    {
        printf("%d | ",i);
        for(int j=1; j<=9; j++)
        {
            printf("%3d",i*j);
        }
        printf("\n");
    }
}

void exam_square()
{
    int input = 0;
    int w, h;
    printf("정사각형 : 1, 직사각형 : 2\n");
    printf("입력 : ");
    scanf("%d",&input);
    
    switch (input) {
        case 1:
            printf("input : ");
            scanf("%d",&w);
            for(int i=0; i<w; i++)
            {
                for(int j=0; j<w; j++)
                {
                    printf("*");
                }
                printf("\n");
            }
            break;
        case 2:
            
            printf("높이 : ");
            scanf("%d",&h);
            printf("너비 : ");
            scanf("%d",&w);
            for(int i=0; i<h; i++)
            {
                for(int j=0; j<w; j++)
                {
                    printf("*");
                }
                printf("\n");
            }
            break;
        default:
            break;
    }
}

void exam_triangle()
{
    
}
void exam_spira()
{
    int n;
    printf("피라미드 n단 (n을 입력하세요): ");
    scanf("%d",&n);
    
    for(int i=1; i<=n; i++)
    {
        for(int j=n-i; j>=0; j--)
        {
            printf(" ");
        }
        for(int k=1; k<=(2*i-1); k++)
        {
            printf("*");
        }
        printf("\n");
    }
    
}
void exam_nrpira()
{
    int n;
    printf("숫자 피라미드 n단 (n을 입력하세요): ");
    scanf("%d",&n);
    
    int temp = n;
    
    for(int i=1; i<=n; i++)
    {
        for(int j=1; j<i; j++)
        {
            printf(" ");
        }
        // int result = (2*temp) -1;
        for(int k=1; k<=(2*temp-1); k++)
        {
            printf("%d",i);
        }
        
        temp--;
        printf("\n");
    }

}
