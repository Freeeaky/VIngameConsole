// =================================================================================
// Includes 9
// This file includes all files related to D3D9 & its Renderer
// =================================================================================
#include "IRenderer.h"
#include "thirdparty/DirectX/Include/d3d9.h"
#include "thirdparty/DirectX/Include/d3dx9.h"

// =================================================================================
// Renderer Setup
// =================================================================================
struct D3D9Renderer_SetupData
{
	LPDIRECT3DDEVICE9 pDevice;
};