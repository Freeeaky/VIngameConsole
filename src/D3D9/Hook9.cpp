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
	// Setup Renderer
	if (g_pRenderer == NULL)
	{
		// Setup Data
		D3D9Renderer_SetupData* pSetupData = new D3D9Renderer_SetupData();
		pSetupData->pDevice = pDevice;

		// Setup
		g_pRenderer = new D3D9Renderer();
		g_pRenderer->Setup((void*)pSetupData);
	}

	// Render
	VIngameConsole::Render();

	// Done
	return pD3D9_EndScene(pDevice);
}

// =================================================================================
// Hook 
// =================================================================================
bool VIngameConsole::HookD3D9(HWND hGameWindow)
{
	printf("1\n");

	// Import
	imp_Direct3DCreate9 = (Direct3DCreate9_t)GetProcAddress(GetModuleHandle("D3D9.dll"), "Direct3DCreate9");

	// Present Parameters
	D3DPRESENT_PARAMETERS FakePresentParams;
	ZeroMemory(&FakePresentParams, sizeof(FakePresentParams));
	FakePresentParams.Windowed = true;
	FakePresentParams.SwapEffect = D3DSWAPEFFECT_DISCARD;
	FakePresentParams.hDeviceWindow = hGameWindow;

	printf("2\n");

	// Direct3D
	IDirect3D9* pD3D9 = imp_Direct3DCreate9(D3D_SDK_VERSION);
	if (pD3D9 == NULL)
	{
		MessageBox(NULL, "Failed to create DirectX 9!", "VIngameConsole", NULL);
		return false;
	}

	printf("3\n");

	DWORD s = GetTickCount();

	// Device
	IDirect3DDevice9* pFakeDevice = NULL;
	if (FAILED(pD3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hGameWindow, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &FakePresentParams, &pFakeDevice)))
	{
		printf("3 shit\n");
		MessageBox(NULL, "shit!", "VIngameConsole", NULL);
		MessageBox(NULL, "Failed to create DirectX 9 Device!", "VIngameConsole", NULL);
		
		return false;
	}

	printf("4 took %i\n", GetTickCount() - s);

	// VTable
	DWORD64* pD3D9_DeviceVTable = (DWORD64*)pFakeDevice;
	pD3D9_DeviceVTable = (DWORD64*)pD3D9_DeviceVTable[0];

	printf("5\n");

	// Hook
	Memory::HookFunction(pD3D9_DeviceVTable[42], &D3D9_EndScene_Hook, (void**)&pD3D9_EndScene);

	printf("6\n");

	// Release
	pFakeDevice->Release();
	pD3D9->Release();

	printf("7\n");

	// Done
	return true;
}