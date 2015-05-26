// =================================================================================
// Includes 
// =================================================================================
#include "Includes.h"
#include "Memory/Memory.h"
#include "VIngameConsole.h"
#include "UTIL/UTIL.h"

// =================================================================================
// Init (Thread)
// =================================================================================
void ThreadInit()
{
	UTIL::Attach_Console(true, 0, 0);
	Memory::Init();

	bool bD3D9 = false;
	bool bD3D11 = false;

	while (!bD3D11)
	{
		// Game Window
		// TODO: Change this
		HWND hGameWindow = GetForegroundWindow();

		// D3D11
		if (GetModuleHandle("D3D11.dll") != NULL && !bD3D11)
			bD3D11 = VIngameConsole::HookD3D11(hGameWindow);

		/*if (GetModuleHandle("D3D9.dll") != NULL && !bD3D9)
		{
			bD3D9 = VIngameConsole::HookD3D9(hGameWindow);
		}*/

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