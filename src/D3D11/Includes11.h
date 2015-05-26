// =================================================================================
// Includes 11
// This file includes all files related to D3D11 & its Renderer
// =================================================================================
#include "IRenderer.h"
#include "thirdparty/DirectX/Include/d3d11.h"
#include "FW1FontWrapper/src/FW1FontWrapper.h"

// =================================================================================
// Renderer Setup
// =================================================================================
struct D3D11Renderer_SetupData
{
	ID3D11Device* pDevice;
	ID3D11DeviceContext* pContext;
};