//
//  main.c
//  ch03
//
//  Created by winplz on 2018. 8. 26..
//  Copyright © 2018년 winplz. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>


/* 요소의 개수가 n인 배열 a에서 key와 일치하는 요소를 선형 검색 */
int search(int a[], int n, int key);
void process_print(int a[], int n, int current);
int main(int argc, const char * argv[]) {
    // insert code here...
    
    int i, n, key, idx;
    int *arr;
    
    puts("선형 검색(보초법)");
    printf("요소 개수 : ");
    scanf("%d", &n);
    
    // 배열 크기 초기화
    arr = calloc(n+1, sizeof(int));
    
    
    // 배열에 n 만큼 입력
    for(i=0; i<n; i++)
    {
        printf("arr[%d] : " , i);
        scanf("%d",&arr[i]);
    }
    
    printf("검색값 : ");
    scanf("%d",&key);
    
    idx = search(arr, n, key);
    

    if(idx == -1)
        printf("검색에 실패했습니다.");
    else
        printf("%d는 arr[%d]에 있습니다.\n",key,idx);
    
    free(arr);
    return 0;
}


int search(int a[], int n, int key)
{
    int i =0;
    a[n] = key;
    /*
    while(1)
    {
        if(a[i] == key)
            break;
        i++;
    }
     */

    // -1 == 처음 출력
    process_print(a,n,-1);
    
    //while --> for
    for(i=0; i<n; i++)
    {
        process_print(a, n, i);
        if(a[i] == key)
            break;
    }
    return i == n ? -1 : i; // i == n 이면 -1, 아니면 i
}

void process_print(int a[], int n, int current)
{
    int i;
    if(current == -1)
    {
        for(i=0; i<n; i++)
        {
            if(i==0)
            {
                printf("   |%2d",i);
            }else
                printf("%2d",i);
        }
        printf("\n");
        for(i=0; i<n; i++)
        {
            if(i==0)
                printf("---+");
            else
                printf("---");
        }
        printf("\n");
    }else
    {
        // '*' 위치 출력
        printf("   |");
        for(i=0; i<n; i++)
        {
            if(i==current)
            {
                printf(" *");
                break;
            }
            else
                printf("  ");
        }
        printf("\n");
        // process
        printf("%2d |",current);
        for(i=0; i<n; i++)
        {
            printf("%2d",a[i]);
        }
        printf("\n");
    }
}
