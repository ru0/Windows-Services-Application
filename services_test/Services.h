#include <windows.h>
#include <stdio.h>
#include "Payload.h"

void InstallService(); //��װ����
void RunService();     //��������
                       //ж�ط���
void WINAPI SvcMain(DWORD dwArgc,LPTSTR *lpArgv); //�������
void WINAPI SvcCtrlHandler(DWORD dwCode);         //������ƺ���
void initSvc();
