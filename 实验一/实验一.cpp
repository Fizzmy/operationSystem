// proccreate��Ŀ
#include <windows.h>
#include <iostream>
#include <stdio.h>
#include<cstdlib>

// 
void StartClone(int nCloneID)
{
    // ��ȡ��ǰ�ļ��ľ���·�� 
    TCHAR szFilename[MAX_PATH] ;
    :: GetModuleFileName(NULL, szFilename, MAX_PATH) ;


    // ���ļ�·����nCloneID�����ַ������Ա㴫���ӽ��� 
    TCHAR szCmdLine[MAX_PATH];
	:: sprintf(szCmdLine,"\"%s\" %d",szFilename,nCloneID);
	
	//std::cout<<szCmdLine<<std::endl; 
	// ָ���³����д��ڵ���� 
    STARTUPINFO si;
    :: ZeroMemory(reinterpret_cast <void*> (&si) , sizeof(si) ) ;//���������һ���ڴ����� 
    si.cb = sizeof(si) ;				// �����Ǳ��ṹ�Ĵ�С

    // ���ص������ӽ��̵Ľ�����Ϣ
    PROCESS_INFORMATION pi;

    // ����ͬ���Ŀ�ִ���ļ��������д������̣����������ӽ��̵�����
    BOOL bCreateOK=::CreateProcess(
		szFilename,					// �������EXE��Ӧ�ó��������
		szCmdLine,					// ��������Ϊ��һ���ӽ��̵ı�־
		NULL,						// ȱʡ�Ľ��̰�ȫ��
		NULL,						// ȱʡ���̰߳�ȫ��
		FALSE,						// ���̳о��
		CREATE_NEW_CONSOLE,			// ʹ���µĿ���̨
		NULL,						// �µĻ���
		NULL,						// ��ǰĿ¼
		&si,						// ������Ϣ
		&pi) ;						// ���صĽ�����Ϣ

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
    // ��ʾ����λ��
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

    // ����ֹ֮ǰ��ͣһ�� (l/2��)
    :: Sleep(500) ;
	//system("pause");
    return 0;
}
