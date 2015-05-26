// =================================================================================
// Includes 
// =================================================================================
#include "Includes.h"
#include "VIngameConsole.h"
#include "Includes11.h"

// =================================================================================
// Setup 
// =================================================================================
void D3D11Renderer::Setup(void* pSetupData_)
{
	D3D11Renderer_SetupData* pSetupData = (D3D11Renderer_SetupData*) pSetupData_;
	m_pDevice = pSetupData->pDevice;
	m_pContext = pSetupData->pContext;

	// Font Factory
	m_pFontFactory = NULL;
	FW1CreateFactory(FW1_VERSION, &m_pFontFactory);

	// Font Wrapper
	m_pFontFactory->CreateFontWrapper(m_pDevice, L"Arial", &m_pFontWrapper);
}

// =================================================================================
// Text 
// =================================================================================
void D3D11Renderer::DrawText(int x, int y, Color col, string sText)
{
	DrawText(FONT_NORMAL, x, y, col, sText);
}

void D3D11Renderer::DrawText(eFonts eFont, int x, int y, Color col, string sText)
{
	wstring temp(sText.begin(), sText.end());
	m_pFontWrapper->DrawString(m_pContext, temp.c_str(), float(FontSizes[eFont]), x, y, col.ToUINT32(), FW1_RESTORESTATE);
}