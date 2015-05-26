// =================================================================================
// Includes
// Thanks http://www.unknowncheats.me/forum/d3d-tutorials-and-source/88369-universal-d3d11-hook.html
// =================================================================================
#include "Includes.h"
#include "Memory/Memory.h"
#include "VIngameConsole.h"
#include "Includes11.h"

// =================================================================================
// Import: D3D11CreateDeviceAndSwapChain
// =================================================================================
typedef HRESULT (* D3D11CreateDeviceAndSwapChain_t)(
	_In_opt_        IDXGIAdapter         *pAdapter,
	D3D_DRIVER_TYPE      DriverType,
	HMODULE              Software,
	UINT                 Flags,
	_In_opt_  const D3D_FEATURE_LEVEL    *pFeatureLevels,
	UINT                 FeatureLevels,
	UINT                 SDKVersion,
	_In_opt_  const DXGI_SWAP_CHAIN_DESC *pSwapChainDesc,
	_Out_opt_       IDXGISwapChain       **ppSwapChain,
	_Out_opt_       ID3D11Device         **ppDevice,
	_Out_opt_       D3D_FEATURE_LEVEL    *pFeatureLevel,
	_Out_opt_       ID3D11DeviceContext  **ppImmediateContext
);
D3D11CreateDeviceAndSwapChain_t imp_D3D11CreateDeviceAndSwapChain = NULL;

// =================================================================================
// Variables 
// =================================================================================
ID3D11Device* pD3D11_Device = NULL;
ID3D11DeviceContext* pD3D11_Context = NULL;

typedef HRESULT(__stdcall* D3D11Present_t) (IDXGISwapChain* pThis, UINT SyncInterval, UINT Flags);
D3D11Present_t pD3D11_Present = NULL;

// =================================================================================
// Present Hook 
// =================================================================================
HRESULT __stdcall D3D11_Present_Hook(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
	// Setup Renderer
	if (g_pRenderer == NULL)
	{
		// Context
		pSwapChain->GetDevice(__uuidof(pD3D11_Device), (void**)&pD3D11_Device);
		pD3D11_Device->GetImmediateContext(&pD3D11_Context);

		// Setup Data
		D3D11Renderer_SetupData* pSetupData = new D3D11Renderer_SetupData();
		pSetupData->pContext = pD3D11_Context;
		pSetupData->pDevice = pD3D11_Device;

		// Setup
		g_pRenderer = new D3D11Renderer();
		g_pRenderer->Setup((void*)pSetupData);
	}

	// Render
	VIngameConsole::Render();

	// Done
	return pD3D11_Present(pSwapChain, SyncInterval, Flags);
}


// =================================================================================
// Hook 
// =================================================================================
bool VIngameConsole::HookD3D11(HWND hGameWindow)
{
	// Import
	imp_D3D11CreateDeviceAndSwapChain = (D3D11CreateDeviceAndSwapChain_t) GetProcAddress(GetModuleHandle("D3D11.dll"), "D3D11CreateDeviceAndSwapChain");

	// Swapchain Desc
	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = hGameWindow;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.Windowed = TRUE;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// Swapchain
	IDXGISwapChain* pSwapChain;
	if (FAILED(imp_D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		NULL,
		&featureLevel,
		1,
		D3D11_SDK_VERSION,
		&swapChainDesc,
		&pSwapChain,
		&pD3D11_Device,
		NULL,
		&pD3D11_Context)
	))
	{
		MessageBox(hGameWindow, "Failed to create DirectX 11 Device and Swapchain!", "VIngameConsole", MB_ICONERROR);
		return false;
	}

	// Swapchain VTable
	DWORD64* pD3D11_SwapChainVTable = (DWORD64*)pSwapChain;
	pD3D11_SwapChainVTable = (DWORD64*)pD3D11_SwapChainVTable[0];

	// Hook
	Memory::HookFunction((DWORD64)pD3D11_SwapChainVTable[8], &D3D11_Present_Hook, (void**)&pD3D11_Present);

	// Release
	pD3D11_Device->Release();
	pD3D11_Context->Release();
	pSwapChain->Release();

	// Done
	return true;
}