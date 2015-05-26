// =================================================================================
// Includes 
// =================================================================================
#include "Includes.h"
#include "Memory/Memory.h"
#include "VIngameConsole.h"
#include "Includes9.h"

// =================================================================================
// Import: Direct3DCreate9
// =================================================================================
typedef IDirect3D9* (* Direct3DCreate9_t) (
	UINT SDKVersion
);
Direct3DCreate9_t imp_Direct3DCreate9 = NULL;

// =================================================================================
// Variables
// =================================================================================
typedef HRESULT(__stdcall* D3D9EndScene_t)(LPDIRECT3DDEVICE9 pDevice);
D3D9EndScene_t pD3D9_EndScene = NULL;

// =================================================================================
// EndScene Hook 
// =================================================================================
HRESULT __stdcall D3D9_EndScene_Hook(LPDIRECT3DDEVICE9 pDevice)
{
	
	return pD3D9_EndScene(pDevice);
}

// =================================================================================
// Hook 
// =================================================================================
bool VIngameConsole::HookD3D9(HWND hGameWindow)
{
	// Import
	imp_Direct3DCreate9 = (Direct3DCreate9_t)GetProcAddress(GetModuleHandle("D3D9.dll"), "Direct3DCreate9");

	// Present Parameters
	D3DPRESENT_PARAMETERS FakePresentParams = {
		0,
		0,
		D3DFMT_UNKNOWN,
		0,
		D3DMULTISAMPLE_NONE,
		0,
		D3DSWAPEFFECT_DISCARD,
		0,
		1,
		0,
		D3DFMT_UNKNOWN,
		0,
		0,
		0
	};

	// Direct3D
	IDirect3D9* pD3D9 = imp_Direct3DCreate9(D3D_SDK_VERSION);
	if (pD3D9 == NULL)
	{
		MessageBox(hGameWindow, "Failed to create DirectX 9!", "VIngameConsole", MB_ICONERROR);
		return false;
	}

	// Device
	IDirect3DDevice9* pFakeDevice = NULL;
	if (FAILED(pD3D9->CreateDevice(0, D3DDEVTYPE_HAL, hGameWindow, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &FakePresentParams, &pFakeDevice)))
	{
		MessageBox(hGameWindow, "Failed to create DirectX 9 Device!", "VIngameConsole", MB_ICONERROR);
		return false;
	}

	// VTable
	DWORD64* pD3D9_DeviceVTable = (DWORD64*)pFakeDevice;
	pD3D9_DeviceVTable = (DWORD64*)pD3D9_DeviceVTable[0];

	// Hook
	Memory::HookFunction(pD3D9_DeviceVTable[42], &D3D9_EndScene_Hook, (void**)&pD3D9_EndScene);

	// Release
	pFakeDevice->Release();
	pD3D9->Release();

	// Done
	return true;
}