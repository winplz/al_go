#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

/* signal() */
void (*exit_func)(int);
void sig_handler01(int signum)
{
	printf("signal() : %d\n", signum);
	signal(SIGINT, exit_func); // or signal(SIGINT, SIG_DFL);
}

/* sigaction */
// volatile 키워드 : 읽기/쓰기를 위해 메모리 액세스 필요.
static volatile int g_exit_flag = 0;
static void sig_handler02(int signum)
{
	printf("signal() : %d\n", signum);
	g_exit_flag = 1;
}


void sig_int(int signo)
{
	printf("SIGNAL : SIGINT\n");
	g_exit_flag++;
	if(g_exit_flag == 1)
		signal(SIGINT, SIG_DFL);
}

int main()
{
	// 1. signal() 사용
	// SIGINT 시그널에 대해 지정된 함수(exit_func)를 호출하도록 합니다.
	//exit_func = signal(SIGINT, sig_handler01);

	// 2. sigaction 사용
	/*
	struct sigaction sig_act;
	memset(&sig_act, '\0', sizeof(struct sigaction)); 	// sizeof() 사용시, type을 지정
	sig_act.sa_handler = &sig_handler02;
	if(sigaction(SIGINT, &sig_act, NULL) < 0)
	{
		perror("sigaction");
		return 1;
	}

	struct sigaction intsig;
	printf("PID : %d\n", getpid()); 	// pid 값 출력
	intsig.sa_handler = sig_int; 	// 시그널 핸들러 등록
	sigemptyset(&intsig.sa_mask);	// 시그널 마스크 초기화	
	intsig.sa_flags = 0;

	if(sigaction(SIGINT, &intsig, 0) == -1)
	{
		perror("sigaction");
		return -1;
	}


	while(1)
	{
		// 프로그램이 바로 종료되지 않게 합니다.

		// 2. sigaction
		// siganl 발생 시, g_exit_flag 값이 변경되면 
		if(g_exit_flag >= 5)
		{
			printf("Change flag!!!\n");
			break;
		}
	}
	*/

	
	printf("PID : %d\n", getpid()); 	// pid 값 출력

	struct sigaction intsig;

	intsig.sa_handler = sig_int; 		// 시그널 핸들러 등록
	sigemptyset(&intsig.sa_mask);		// 시그널 마스크 초기화	
	sigfillset(&intsig.sa_mask);		// 모든 시그널 등록
	sigdelset(&intsig.sa_mask, SIGINT);	// SIGINT 시그널 제외
	intsig.sa_flags = 0;
	
	if(sigprocmask(SIG_BLOCK, &intsig.sa_mask, 0) < 0)
	{
		perror("sigmask");
		return -1;
	}

	if(sigaction(SIGINT, &intsig, 0) < 0)
	{
		perror("sigaction");
		return -1;
	}



	while(1)
	{
	}

	return 0;
}



