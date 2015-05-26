// =================================================================================
// Includes 
// =================================================================================
#include "Includes.h"
#include "Memory/Memory.h"
#include "VIngameConsole.h"

// =================================================================================
// Init (Thread)
// =================================================================================
void ThreadInit()
{
	Memory::Init();

	while (true)
	{
		// Game Window
		// Maybe find a better way?
		HWND hGameWindow = GetForegroundWindow();

		// D3D11
		if (GetModuleHandle("D3D11.dll") != NULL)
			if (VIngameConsole::HookD3D11(hGameWindow)) break;

		//if (GetModuleHandle("D3D9.dll") != NULL)
		//	if (VIngameConsole::HookD3D9()) break;

		Sleep(200);
	}
}

// =================================================================================
// DLLMain 
// =================================================================================
BOOL __stdcall DllMain(HINSTANCE hModule, DWORD dwReason, LPVOID lpReserved)
{
	// Startup
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE) ThreadInit, 0, 0, 0);
	}

	// Cleanup
	if (dwReason == DLL_PROCESS_DETACH)
	{
	
	}

	// Success
	return TRUE;
}