#if 0  //顺序执行
#include <stdio.h>
#include <Windows.h>

int main_martin(int cnt) {
	for (int i = 0; i < cnt; i++) {
		Sleep(3000);
		printf("martin 俯卧撑	");
	}
	return cnt;
}

int main_rock(int cnt) {
	for (int i = 0; i < cnt; i++) {
		Sleep(1000);
		printf("rock 甩头发	");
	}
	return cnt;
}
int main_xixi(int cnt) {
	for (int i = 0; i < cnt; i++) {
		Sleep(2000);
		printf("xixi 唱歌	");
	}
	return cnt;
}

int main(void) {
	int martin = 20;
	int rock = 50;
	int xixi = 40;

	main_martin(martin);
	printf("\n");
	main_rock(rock);
	printf("\n");
	main_xixi(xixi);
	printf("\n");

	system("pause");
	return 0;
}
#endif

#if 0
/*
//多线程 不同时多执行
unsigned long _beginthreadex(
	void* security,									// 安全属性， 为NULL时表示默认安全性
	unsigned stack_size,							// 线程的堆栈大小， 一般默认为0
	unsigned(_stdcall* start_address)(void*),		// 线程函数
	void* argilist,									// 线程函数的参数
	unsigned initflag,								// 新线程的初始状态，0表示立即执行，CREATE_SUSPENDED表示创建之后挂起
	unsigned* threaddr);							// 用来接收线程ID
//返回值 :  成功返回新线程句柄， 失败返回0
*/

unsigned int __stdcall thread_main_martin(void* arg) {
	int cnt = *((int*)arg);
	for (int i = 0; i < cnt; i++) {
		Sleep(1000);
		printf("\nmartin 俯卧撑\n");
	}
	return 0;
}

unsigned int __stdcall thread_main_rock(void* arg) {
	int cnt = *((int*)arg);
	for (int i = 0; i < cnt; i++) {
		Sleep(1000);
		printf("\nrock 甩头发\n");
	}
	return 0;
}

unsigned int __stdcall thread_main_xixi(void* arg) {
	int cnt = *((int*)arg);
	for (int i = 0; i < cnt; i++) {
		Sleep(1000);
		printf("\nxixi 唱歌\n");
	}
	return 0;
}

int main(void) {
	int martin = 20;
	int rock = 50;
	int xixi = 40;
	unsigned int martin_id, rock_id, xixi_id;

	_beginthreadex(NULL, 0, thread_main_martin, (void*)&martin, 0, &martin_id);
	_beginthreadex(NULL, 0, thread_main_rock, (void*)&rock, 0, &rock_id);
	_beginthreadex(NULL, 0, thread_main_xixi, (void*)&xixi, 0, &xixi_id);
	
	printf("martin_id:%d", martin_id);
	printf("	rock_id:%d", rock_id);
	printf("	xixi_id:%d\n", xixi_id);

	system("pause");
	return 0;
}
#endif // 0

#if 0
#include <stdio.h>
#include <Windows.h>
#include <process.h>

/*
HANDLE CreateThread(
LPSECURITY_ATTRIBUTES lpThreadAttributes,	//SD
SIZE_T dwStackSize,							//initialstacksize
LPTHREAD_START_ROUTINE lpStartAddress,		//threadfunction
LPVOID lpParameter,							//threadargument
DWORD dwCreationFlags,						//creationoption
LPDWORD lpThreadId							//threadidentifier
)
必须通过CloseHandle函数来关闭该线程对象。

第一个参数 lpThreadAttributes 表示线程内核对象的安全属性，一般传入NULL表示使用默认设置。
第二个参数 dwStackSize 表示线程栈空间大小。传入0表示使用默认大小（1MB）。
第三个参数 lpStartAddress 表示新线程所执行的线程函数地址，多个线程可以使用同一个函数地址。
第四个参数 lpParameter 是传给线程函数的参数。
第五个参数 dwCreationFlags 指定额外的标志来控制线程的创建，为0表示线程创建之后立即就可以进行调度，如果为CREATE_SUSPENDED则表示线程创建后暂停运行，这样它就无法调度，直到调用ResumeThread()。
第六个参数 lpThreadId 将返回线程的ID号，传入NULL表示不需要返回该线程ID号
*/

DWORD WINAPI ThreadFun(LPVOID p) {
	int iMym = *((int*)p);
	printf("\n我是子线程, PID = %d  iMym = %d\n", GetCurrentThreadId(), iMym);
	return 0;
}

int main(void) {
	HANDLE hThread = NULL;
	DWORD dwThreadID;
	int m = 100;
	hThread = CreateThread(NULL, 0, ThreadFun, &m, 0, &dwThreadID);
	printf("我是主线程, PID = %d\n", GetCurrentThreadId());
	CloseHandle(hThread);

	system("pause");
	return 0;
}

#endif // 0

#if 0	//线程等待时间
#include <stdio.h>
#include <windows.h>
#include <process.h>

unsigned WINAPI ThreadFunc(void* arg)
{
	int i;
	int cnt = *((int*)arg);
	for (i = 0; i < cnt; i++)
	{
		printf("running thread\n");
		Sleep(1000);
	}
	return 0;
}

/*
 WaitForSingleObject
 来等待一个内核对象变为已通知状态

 WaitForSingleObject(
	_In_ HANDLE hHandle,		  //指明一个内核对象的句柄
	_In_ DWORD dwMilliseconds     //等待时间
);
*/
int main(int argc, char* argv[])
{
	HANDLE hThread;
	unsigned threadID;
	int param = 5;
	DWORD wr;

	printf("mian begin\n");

	hThread = (HANDLE)_beginthreadex(NULL, 0, ThreadFunc, (void*)&param, 0, &threadID);
	if (hThread == NULL)
	{
		printf("_beginthreadex() error\n");
		return -1;
	}

	printf("WaitForSingleObject begin\n");
	if ((wr = WaitForSingleObject(hThread, INFINITE)) == WAIT_FAILED)
	{
		printf("thread wait error");
		return -1;
	}
	printf("WaitForSingleObject end\n");

	//Sleep(3000);
	printf("main end\n");

	return 0;
}
#endif


#if 0
//起两个线程，一个加+1，一个减1
#include <stdio.h>
#include <windows.h>
#include <process.h>

#define NUM_THREAD	50
unsigned WINAPI threadInc(void* arg);
unsigned WINAPI threadDes(void* arg);
long long num = 0;
/*
WaitForMultipleObjects(
	_In_ DWORD nCount,								// 要监测的句柄的组的句柄的个数
	_In_reads_(nCount) CONST HANDLE* lpHandles,     //要监测的句柄的组
	_In_ BOOL bWaitAll,								// TRUE 等待所有的内核对象发出信号， FALSE 任意一个内核对象发出信号
	_In_ DWORD dwMilliseconds						//等待时间
	);
*/
int main(int argc, char* argv[])
{
	HANDLE tHandles[NUM_THREAD];
	int i;

	printf("sizeof long long: %d \n", sizeof(long long));
	for (i = 0; i < NUM_THREAD; i++)
	{
		if (i % 2)
			tHandles[i] = (HANDLE)_beginthreadex(NULL, 0, threadInc, NULL, 0, NULL);
		else
			tHandles[i] = (HANDLE)_beginthreadex(NULL, 0, threadDes, NULL, 0, NULL);
	}

	WaitForMultipleObjects(NUM_THREAD, tHandles, TRUE, INFINITE);
	printf("result: %lld \n", num);

	//printf("pause");
	return 0;
}

unsigned WINAPI threadInc(void* arg)
{
	int i;
	for (i = 0; i < 50000; i++)
		num += 1;
	return 0;
}
unsigned WINAPI threadDes(void* arg)
{
	int i;
	for (i = 0; i < 50000; i++)
		num -= 1;
	return 0;
}
#endif //0

#if 0
//线程同步-互斥对象
#include <stdio.h>
#include <windows.h>
#include <process.h>

#define NUM_THREAD	50
unsigned WINAPI threadInc(void* arg);
unsigned WINAPI threadDes(void* arg);

long long num = 0;
HANDLE hMutex;	//互斥体对象

/*
HANDLE
WINAPI
CreateMutexW(
	_In_opt_ LPSECURITY_ATTRIBUTES lpMutexAttributes,   //指向安全属性
	_In_ BOOL bInitialOwner,							//初始化互斥对象的所有者  TRUE 立即拥有互斥体
	_In_opt_ LPCWSTR lpName								//指向互斥对象名的指针  L“Bingo”
	);

WaitForSingleObject										//等待 HANDLE对象的通知
*/

int main(int argc, char* argv[])
{
	HANDLE tHandles[NUM_THREAD];
	int i;

	hMutex = CreateMutex(NULL, FALSE, NULL);
	for (i = 0; i < NUM_THREAD; i++)
	{
		if (i % 2)
			tHandles[i] = (HANDLE)_beginthreadex(NULL, 0, threadInc, NULL, 0, NULL);
		else
			tHandles[i] = (HANDLE)_beginthreadex(NULL, 0, threadDes, NULL, 0, NULL);
	}

	WaitForMultipleObjects(NUM_THREAD, tHandles, TRUE, INFINITE);
	CloseHandle(hMutex);
	printf("result: %lld \n", num);
	return 0;
}

unsigned WINAPI threadInc(void* arg)
{
	int i;

	WaitForSingleObject(hMutex, INFINITE);	//临界区 是一段代码段 不是变量
	for (i = 0; i < 50000; i++)
		num += 1;
	ReleaseMutex(hMutex);

	return 0;
}
unsigned WINAPI threadDes(void* arg)
{
	int i;

	WaitForSingleObject(hMutex, INFINITE);
	for (i = 0; i < 50000; i++)
		num -= 1;
	ReleaseMutex(hMutex);

	return 0;
}
#endif //0