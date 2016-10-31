#include "Services.h"

SERVICE_STATUS ServiceStatus;
SERVICE_STATUS_HANDLE ServiceStatusHandle;

#define SVCNAME TEXT("WDevmgmtSvc")
#define SVCDISNAME TEXT("Windows Devices Management Service")
#define szDesc TEXT("Windows Device Manager to install and update the drivers for hardware devices, change the hardware settings for those devices, and troubleshoot problems.");

void initSvc(){

	SERVICE_TABLE_ENTRY DispatchTable[] = 
	{ 
		{ SVCNAME, (LPSERVICE_MAIN_FUNCTION) SvcMain }, 
		{ NULL, NULL }
	};

	// This call returns when the service has stopped. 

	if (!StartServiceCtrlDispatcher( DispatchTable ))
    {
		printf("StartServiceCtrlDispatcher failed (%d)\n", GetLastError());
    }

}



//��װ����
void InstallService()
{
	SC_HANDLE schSCManager;
	SC_HANDLE schService;
	SERVICE_DESCRIPTION sd;
	TCHAR szPath[MAX_PATH]; //�ļ�·��

	// Change the service description.
    sd.lpDescription = szDesc;


    if( !GetModuleFileName(NULL, szPath, MAX_PATH ) )
    {
        printf("Cannot install service (%d)\n", GetLastError());
        return;
    }

	// Get a handle to the SCM database. 
	schSCManager = OpenSCManager(
		NULL, 
		NULL, 
		SC_MANAGER_ALL_ACCESS);

	if (NULL == schSCManager)
    {
        printf("OpenSCManager failed (%d)\n", GetLastError());
        return;
    }

	// Create the service

	schService = CreateService(
		schSCManager,
		SVCNAME,                    // name of service 
		SVCDISNAME,                 // lpDisplayName
		SERVICE_ALL_ACCESS,
		SERVICE_WIN32_OWN_PROCESS,  // �������� ��ռ
		SERVICE_AUTO_START,
		SERVICE_ERROR_IGNORE,
		szPath,                     // path to service's binary 
		NULL,
		NULL,
		NULL,
		NULL,
		NULL);

	if (schService == NULL)
    {
        printf("CreateService failed (%d)\n", GetLastError()); 
        CloseServiceHandle(schSCManager);
        return;
    }
    else printf("Service installed successfully\n"); 

	//Changes the optional configuration parameters of a service.

	if (ChangeServiceConfig2(
		schService, 
        SERVICE_CONFIG_DESCRIPTION,
        &sd) == FALSE)
	{
		OutputDebugString("*Warning Change the services description failed.");
	}

    CloseServiceHandle(schService); 
    CloseServiceHandle(schSCManager);
}


void RunService()
{
	//SERVICE_STATUS_PROCESS ssStatus;
	// Get a handle to the SCM database. 
 
    SC_HANDLE schSCManager = OpenSCManager( 
        NULL,                    // local computer
        NULL,                    // servicesActive database 
        SC_MANAGER_ALL_ACCESS);  // full access rights 
 
    if (NULL == schSCManager) 
    {
        printf("OpenSCManager failed (%d)\n", GetLastError());
        return;
    }

    // Get a handle to the service.

    SC_HANDLE schService = OpenService( 
        schSCManager,         // SCM database 
        SVCNAME,              // name of service 
        SERVICE_ALL_ACCESS);  // full access 
 
    if (schService == NULL)
    { 
        printf("OpenService failed (%d)\n", GetLastError()); 
        CloseServiceHandle(schSCManager);
        return;
    }

	//ȥ������״̬������

	// Attempt to start the service.

    if (!StartService(
            schService,  // handle to service 
            0,           // number of arguments 
            NULL) )      // no arguments 
    {
        printf("StartService failed (%d)\n", GetLastError());
        CloseServiceHandle(schService); 
        CloseServiceHandle(schSCManager);
        return;
    }
    else printf("Service start pending...\n");

	// �жϷ����Ƿ�������
	/*
    if (ssStatus.dwCurrentState == SERVICE_RUNNING) 
    {
        printf("Service started successfully.\n"); 
    }
	*/
    CloseServiceHandle(schService); 
    CloseServiceHandle(schSCManager);
}


//������ں���
void WINAPI SvcMain(DWORD dwArgc,LPTSTR *lpArgv)
{
	HANDLE hThread;
	ServiceStatus.dwServiceType      = SERVICE_WIN32;
	ServiceStatus.dwCurrentState      = SERVICE_START_PENDING;
	ServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_PAUSE_CONTINUE;
	ServiceStatus.dwServiceSpecificExitCode      = 0;
	ServiceStatus.dwWin32ExitCode            = 0;
	ServiceStatus.dwCheckPoint            = 0;
	ServiceStatus.dwWaitHint            = 0;

	ServiceStatusHandle = RegisterServiceCtrlHandler(
		SVCNAME, 
		SvcCtrlHandler);
	
	if(!ServiceStatusHandle)
	{
        printf("RegisterServiceCtrlHandler faild (%d)\n", GetLastError());
        return;
	}

	//��������״̬
	ServiceStatus.dwCurrentState = SERVICE_RUNNING;
	ServiceStatus.dwCheckPoint = 0;
	ServiceStatus.dwWaitHint = 0;

	if(SetServiceStatus(ServiceStatusHandle, &ServiceStatus) == 0)
	{
        printf("SetServiceStatus faild (%d)\n", GetLastError());
        return;
	}

	//���������߳�   ������ɵĹ������������
	
	hThread = CreateThread(
		NULL,
		0,
		Service,
		NULL,
		0,
		NULL);

	if(hThread == NULL)
	{
		//CreateThread error!
		return;
	}

	CloseHandle(hThread);

}

//�������

void WINAPI SvcCtrlHandler(DWORD dwCode)
{
	switch(dwCode)
	{
		case SERVICE_CONTROL_PAUSE:
			ServiceStatus.dwCurrentState      = SERVICE_PAUSED;
			break;

		case SERVICE_CONTROL_CONTINUE:
			ServiceStatus.dwCurrentState      = SERVICE_RUNNING;
			break;

		case SERVICE_CONTROL_STOP:
			ServiceStatus.dwCurrentState      = SERVICE_STOPPED;
			ServiceStatus.dwWin32ExitCode      = 0;
			ServiceStatus.dwCheckPoint      = 0;
			ServiceStatus.dwWaitHint      = 0;
		
			if(SetServiceStatus(ServiceStatusHandle, &ServiceStatus) == 0)
			{
				printf("ServiceStatusHandle (%d)\n", GetLastError());
			}
			return;
		case SERVICE_CONTROL_INTERROGATE:
			break;
		default:
			break;
	}

	if(SetServiceStatus(ServiceStatusHandle, &ServiceStatus) == 0)
	{
        printf("ServiceStatusHandle (%d)\n", GetLastError());
	}

	return ;
}




