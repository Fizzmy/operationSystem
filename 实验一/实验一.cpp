// proccreate项目
#include <windows.h>
#include <iostream>
#include <stdio.h>
#include<cstdlib>

// 
void StartClone(int nCloneID)
{
    // 获取当前文件的绝对路径 
    TCHAR szFilename[MAX_PATH] ;
    :: GetModuleFileName(NULL, szFilename, MAX_PATH) ;


    // 将文件路径和nCloneID加入字符串，以便传入子进程 
    TCHAR szCmdLine[MAX_PATH];
	:: sprintf(szCmdLine,"\"%s\" %d",szFilename,nCloneID);
	
	//std::cout<<szCmdLine<<std::endl; 
	// 指明新程序中窗口的外观 
    STARTUPINFO si;
    :: ZeroMemory(reinterpret_cast <void*> (&si) , sizeof(si) ) ;//用零来填充一块内存区域 
    si.cb = sizeof(si) ;				// 必须是本结构的大小

    // 返回的用于子进程的进程信息
    PROCESS_INFORMATION pi;

    // 利用同样的可执行文件和命令行创建进程，并赋于其子进程的性质
    BOOL bCreateOK=::CreateProcess(
		szFilename,					// 产生这个EXE的应用程序的名称
		szCmdLine,					// 告诉其行为像一个子进程的标志
		NULL,						// 缺省的进程安全性
		NULL,						// 缺省的线程安全性
		FALSE,						// 不继承句柄
		CREATE_NEW_CONSOLE,			// 使用新的控制台
		NULL,						// 新的环境
		NULL,						// 当前目录
		&si,						// 启动信息
		&pi) ;						// 返回的进程信息

    // 
    if (bCreateOK)
    {
        :: CloseHandle(pi.hProcess) ;
        :: CloseHandle(pi.hThread) ;
    }
}

int main(int argc, char* argv[] )
{
    // 
    int nClone(0) ;
    system("pause");
    //:: Sleep(20000) ;
	//std::cout<<argv[0]<<std::endl; 
    if (argc > 1)
    {
        // 
        
        :: sscanf(argv[1] , "%d" , &nClone) ;
    }
    //system("pause");
	//__asm{int 3}
    // 显示进程位置
    std :: cout << "Process ID:" << :: GetCurrentProcessId()
                << ", Clone ID:" << nClone
                << std :: endl;

    // 
    const int c_nCloneMax=25;
    if (nClone < c_nCloneMax)
    {
        // 
		:: Sleep(500) ;
        StartClone(++nClone) ;
        
    }

    // 在终止之前暂停一下 (l/2秒)
    :: Sleep(500) ;
	//system("pause");
    return 0;
}
