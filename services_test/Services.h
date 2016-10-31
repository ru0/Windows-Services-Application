#include <windows.h>
#include <stdio.h>
#include "Payload.h"

void InstallService(); //安装服务
void RunService();     //启动服务
                       //卸载服务
void WINAPI SvcMain(DWORD dwArgc,LPTSTR *lpArgv); //服务入口
void WINAPI SvcCtrlHandler(DWORD dwCode);         //服务控制函数
void initSvc();
