// =================================================================================
// Includes 
// =================================================================================
#include "Includes.h"
#include "VIngameConsole.h"
#include "Includes9.h"
#include "TextDrawingHelper/RRenderer.h"

// =================================================================================
// Setup 
// =================================================================================
void D3D9Renderer::Setup(void* pSetupData_)
{
	D3D9Renderer_SetupData* pSetupData = (D3D9Renderer_SetupData*) pSetupData_;
	m_pDevice = pSetupData->pDevice;

	// Fonts
	m_pFonts[FONT_SMALL] = new CD3DFont("Arial", FontSizes[FONT_SMALL], 512, 0);
	m_pFonts[FONT_NORMAL] = new CD3DFont("Arial", FontSizes[FONT_NORMAL], 512, 0);
	m_pFonts[FONT_BIG] = new CD3DFont("Arial", FontSizes[FONT_BIG], 512, 0);

	// Init
	m_pFonts[FONT_SMALL]->Initialize(m_pDevice);
	m_pFonts[FONT_NORMAL]->Initialize(m_pDevice);
	m_pFonts[FONT_BIG]->Initialize(m_pDevice);
}

// =================================================================================
// Text 
// =================================================================================
void D3D9Renderer::DrawText(int x, int y, Color col, string sText)
{
	DrawText(FONT_NORMAL, x, y, col, sText);
}

void D3D9Renderer::DrawText(eFonts eFont, int x, int y, Color col, string sText)
{
	m_pFonts[eFont]->Print(float(x), float(y), (DWORD) col.ToUINT32(), const_cast<char*>(sText.c_str()), 0);
}