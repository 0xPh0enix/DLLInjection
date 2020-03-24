/*
    Coded by 0xPh0enix
*/

#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>

typedef struct {

    LPVOID lpDllAddr;
    HANDLE hRemoteThread;

} INJECTION_RESULT;

//Helper function for search process by name
DWORD FindProcessId(LPCSTR pszProcName) {

    PROCESSENTRY32 pEntry;
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

    if (Process32First(hSnap, &pEntry)) {

        while (Process32Next(hSnap, &pEntry)) {

            if (lstrcmp(pEntry.szExeFile, pszProcName) == ERROR_SUCCESS)
                return pEntry.th32ProcessID;

        }

    }

}

//Function for DLL Injection
INJECTION_RESULT InjectDll(HANDLE hProcess, LPCSTR pszDllPath) {

    INJECTION_RESULT irResult = { 0 };

    LPVOID lpAddr = VirtualAllocEx(hProcess, NULL, lstrlen(pszDllPath) + 1, MEM_COMMIT, PAGE_READWRITE);

    irResult.lpDllAddr = lpAddr;

    if (!lpAddr)
        return {0};

    if (!WriteProcessMemory(hProcess, lpAddr, pszDllPath, lstrlen(pszDllPath) + 1, NULL))
        return {0};

    irResult.hRemoteThread = CreateRemoteThread(hProcess, NULL, NULL, (LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA"), lpAddr, NULL, NULL);


    return irResult;

}

int main()
{
     
	
	LPCSTR pszPath2Dll = "C:\\Users\\Admin\\Desktop\\DllInjection\\Payload.dll";

    HANDLE hTarget = OpenProcess(PROCESS_ALL_ACCESS, FALSE, FindProcessId("Target.exe"));

    INJECTION_RESULT irResult = InjectDll(hTarget, pszPath2Dll);

    std::cout << "DLL Path allocated at " << irResult.lpDllAddr;
    std::cin.get();
    
}
