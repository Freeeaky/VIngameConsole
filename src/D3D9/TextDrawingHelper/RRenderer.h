#include "RHeader.h"

//then i go and use this

#define _DEBUG(s) /*s*/

IDirect3DDevice9		*CD3DBaseRender::m_pD3Ddev = NULL;
IDirect3DStateBlock9	*CD3DBaseRender::m_pD3DstateDraw = NULL;
IDirect3DStateBlock9	*CD3DBaseRender::m_pD3DstateNorm = NULL;
int						CD3DBaseRender::m_renderCount = 0;
int						CD3DBaseRender::m_numShared = 0;
bool					CD3DBaseRender::m_statesOK = false;

inline d3dfont_s InitFont2DVertex(float x, float y, DWORD colour, float tu, float tv)
{
	d3dfont_s v = { x, y, 1.0f, 1.0f, colour, tu, tv };
	return v;
}

inline d3dprimitives_s InitPrim2DVertex(float x, float y, DWORD colour)
{
	d3dprimitives_s v = { x, y, 1.0f, 1.0f, colour };
	return v;
}


CD3DBaseRender::CD3DBaseRender()
{
	m_numShared++;
}

CD3DBaseRender::~CD3DBaseRender()
{
	if( (--m_numShared) == 0)
		DeleteStates();
}

HRESULT CD3DBaseRender::Initialize(IDirect3DDevice9 *pD3Ddev)
{
	if( m_pD3Ddev == NULL && (m_pD3Ddev = pD3Ddev) == NULL )
	{
		_DEBUG("m_pD3Ddev is NULL");
		return E_FAIL;
	}
	
	if( !m_statesOK && FAILED(CreateStates()) )
	{
		_DEBUG("CreateStates() failed");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CD3DBaseRender::Invalidate()
{
	DeleteStates();
	return S_OK;		
}

HRESULT CD3DBaseRender::BeginRender()
{
	if( !m_statesOK )
	{
		_DEBUG("::BeginRender() m_statesOK = false");
		return E_FAIL;
	}

	m_renderCount++;
	if( m_renderCount == 1 )
	{
		m_pD3DstateNorm->Capture();
		m_pD3DstateDraw->Apply();
	}

	return S_OK;
}

HRESULT CD3DBaseRender::EndRender()
{
	if( !m_statesOK )
	{
		_DEBUG("::EndRender() m_statesOK = false");
		return E_FAIL;
	}

	m_renderCount--;

	if( m_renderCount == 0)
		m_pD3DstateNorm->Apply();
	else if(m_renderCount < 0)
		m_renderCount = 0;

	return S_OK;
}

HRESULT CD3DBaseRender::CreateStates()
{
    for(int iStateBlock = 0; iStateBlock < 2; iStateBlock++)
	{
		m_pD3Ddev->BeginStateBlock();
		m_pD3Ddev->SetPixelShader(NULL);
		m_pD3Ddev->SetVertexShader(NULL);

        m_pD3Ddev->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
        m_pD3Ddev->SetRenderState( D3DRS_SRCBLEND,   D3DBLEND_SRCALPHA );
        m_pD3Ddev->SetRenderState( D3DRS_DESTBLEND,  D3DBLEND_INVSRCALPHA );
        m_pD3Ddev->SetRenderState( D3DRS_ALPHATESTENABLE,  TRUE );
        m_pD3Ddev->SetRenderState( D3DRS_ALPHAREF,         0x08 );
        m_pD3Ddev->SetRenderState( D3DRS_ALPHAFUNC,  D3DCMP_GREATEREQUAL );
        m_pD3Ddev->SetRenderState( D3DRS_FILLMODE,   D3DFILL_SOLID );
        m_pD3Ddev->SetRenderState( D3DRS_CULLMODE,   D3DCULL_CCW );
        m_pD3Ddev->SetRenderState( D3DRS_STENCILENABLE,    FALSE );
        m_pD3Ddev->SetRenderState( D3DRS_CLIPPING,         TRUE );
        m_pD3Ddev->SetRenderState( D3DRS_CLIPPLANEENABLE,  FALSE );
        m_pD3Ddev->SetRenderState( D3DRS_VERTEXBLEND,      D3DVBF_DISABLE );
        m_pD3Ddev->SetRenderState( D3DRS_INDEXEDVERTEXBLENDENABLE, FALSE );
        m_pD3Ddev->SetRenderState( D3DRS_FOGENABLE,        FALSE );
        m_pD3Ddev->SetRenderState( D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED|D3DCOLORWRITEENABLE_GREEN|D3DCOLORWRITEENABLE_BLUE|D3DCOLORWRITEENABLE_ALPHA);
        m_pD3Ddev->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
        m_pD3Ddev->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
        m_pD3Ddev->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
        m_pD3Ddev->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );
        m_pD3Ddev->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
        m_pD3Ddev->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
        m_pD3Ddev->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, 0 );
        m_pD3Ddev->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE );
        m_pD3Ddev->SetTextureStageState( 1, D3DTSS_COLOROP,   D3DTOP_DISABLE );
        m_pD3Ddev->SetTextureStageState( 1, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );
        m_pD3Ddev->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_POINT );
        m_pD3Ddev->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_POINT );
        m_pD3Ddev->SetSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_NONE );

		if(iStateBlock)	m_pD3Ddev->EndStateBlock(&m_pD3DstateDraw);
		else			m_pD3Ddev->EndStateBlock(&m_pD3DstateNorm);
	}

	m_statesOK = true;

	return S_OK;
}

HRESULT CD3DBaseRender::DeleteStates()
{
	SAFE_RELEASE(m_pD3DstateDraw);
	SAFE_RELEASE(m_pD3DstateNorm);
	m_statesOK = false;

	return S_OK;
}

/*
ABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRS
TUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKL
MNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDE
*/

CD3DFont::CD3DFont(char *szFontName, int fontHeight, int maxCharacters, DWORD dwCreateFlags)
{
	strcpy(m_szFontName, (szFontName ? szFontName : "Arial"));

	m_fontHeight = fontHeight;
	m_dwCreateFlags = dwCreateFlags;

	m_isReady = false;
	m_statesOK = false;

	m_pD3Dtex = NULL;
	m_pD3Dbuf = NULL;

	m_maxTriangles = 96*2;
}

CD3DFont::~CD3DFont()
{
	Invalidate();
}


HRESULT CD3DFont::Initialize(IDirect3DDevice9 *pD3Ddev)
{
	if( FAILED(CD3DBaseRender::Initialize(pD3Ddev)) )
	{
		_DEBUG("::Initialize(pD3Ddev) failed");
		return E_FAIL;
	}

	if( m_pRender == NULL && (m_pRender = new CD3DRender(16)) == NULL)
	{
		_DEBUG("m_pRender = NULL");
		return E_FAIL;
	}

	//if( FAILED(m_pRender->Initialize(pD3Ddev)) )
	//{
	//	_DEBUG("m_pRender->Initialize failed");
	//	return E_FAIL;
	//}

	m_texWidth = m_texHeight = 512;

	if( FAILED(m_pD3Ddev->CreateTexture(m_texWidth, m_texHeight, 1, 0, D3DFMT_A4R4G4B4, D3DPOOL_MANAGED, &m_pD3Dtex, NULL)) )
	{
		_DEBUG("CreateTexture failed");
		return E_FAIL;
	}
	
	if( FAILED(m_pD3Ddev->CreateVertexBuffer(m_maxTriangles*3*sizeof(d3dfont_s), D3DUSAGE_WRITEONLY|D3DUSAGE_DYNAMIC, 0, D3DPOOL_DEFAULT, &m_pD3Dbuf, NULL)) )
	{
		_DEBUG("CreateVertexBuffer failed");
		SAFE_RELEASE(m_pD3Dtex);
		return E_FAIL;
	}

	DWORD *pBitmapBits;
	BITMAPINFO bmi;

	ZeroMemory(&bmi.bmiHeader,  sizeof(BITMAPINFOHEADER));
	bmi.bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth       = m_texWidth;
	bmi.bmiHeader.biHeight      = -m_texHeight;
	bmi.bmiHeader.biPlanes      = 1;
	bmi.bmiHeader.biCompression = BI_RGB;
	bmi.bmiHeader.biBitCount    = 32;

	HDC hDC = CreateCompatibleDC(NULL);
	HBITMAP hbmBitmap = CreateDIBSection(hDC, &bmi, DIB_RGB_COLORS, (void**)&pBitmapBits, NULL, 0);
	SetMapMode(hDC, MM_TEXT);

	int iHeight = -m_fontHeight * (int)GetDeviceCaps(hDC, LOGPIXELSY) / 72;

	HFONT hFont = CreateFont(iHeight, 0, 0, 0, (m_dwCreateFlags&FCR_BOLD ? FW_BOLD : FW_NORMAL), m_dwCreateFlags&FCR_ITALICS, false, false, 
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, 
		VARIABLE_PITCH, m_szFontName);

	if(hFont == NULL)
	{
		_DEBUG("hFont = NULL");
		return E_FAIL;
	}

	SelectObject(hDC, hbmBitmap);
	SelectObject(hDC, hFont);

	SetTextColor(hDC, 0x00FFFFFF);
	SetBkColor(hDC, 0x00000000);
	SetTextAlign(hDC, TA_TOP);

	int x, y = 0;
	char szStr[2] = { ' ', 0 };
	SIZE size;

	GetTextExtentPoint32(hDC, szStr, 1, &size);
	x = m_chrSpacing = (int)ceil(size.cx/4.0f);
	m_fChrHeight = (float)(size.cy-1);

	for(char c = 32; c < 127; c++)
	{
		szStr[0] = c;
		GetTextExtentPoint32(hDC, szStr, 1, &size);

		if( x + size.cx + m_chrSpacing > m_texWidth )
		{
			x = m_chrSpacing;
			y += size.cy + 1;
		}

		RECT rect = {x, y, x + size.cx, y + size.cy };
		ExtTextOut(hDC, x, y, ETO_CLIPPED, &rect, szStr, 1, NULL);

		//tu src + dst
		m_fTexCoords[c-32][0] = (float)(x + 0		- m_chrSpacing) / (float)m_texWidth;
		m_fTexCoords[c-32][2] = (float)(x + size.cx + m_chrSpacing) / (float)m_texWidth;
        
		//tv src + dst
		m_fTexCoords[c-32][1] = (float)(y + 0		+ 0) / (float)m_texHeight;
		m_fTexCoords[c-32][3] = (float)(y + size.cy + 0) / (float)m_texHeight;
        
		x += size.cx + (2*m_chrSpacing);
	}
	
	D3DLOCKED_RECT d3dlr;    
	m_pD3Dtex->LockRect(0, &d3dlr, 0, 0);
    
	BYTE *pDstRow = (BYTE*)d3dlr.pBits;
	WORD *pDst16;
	BYTE bAlpha;

	for(y=0; y < m_texHeight; y++)
	{
		pDst16 = (WORD*)pDstRow;
		
		for(x=0; x < m_texWidth; x++)
		{
			bAlpha = (BYTE)(pBitmapBits[m_texWidth*y+x]>>8) & 0xFF;
			*pDst16 = (WORD)(bAlpha << 8) | 0x0FFF;
			pDst16++;
		}
		
		pDstRow += d3dlr.Pitch;
    }

	m_pD3Dtex->UnlockRect(0);

	DeleteObject(hbmBitmap);
	DeleteDC(hDC);
	DeleteObject(hFont);

	m_isReady = true;

	return S_OK;
}

HRESULT CD3DFont::Invalidate()
{
	CD3DBaseRender::Invalidate();
	m_isReady = false;

	SAFE_RELEASE(m_pD3Dtex);
	SAFE_RELEASE(m_pD3Dbuf);

	m_pRender->Invalidate();

	return S_OK;
}

HRESULT CD3DFont::Print(float x, float y, DWORD colour, char *szText, DWORD dwFlags)
{
	if(!m_isReady)
	{
		_DEBUG("::Print() m_isReady = false");
		return E_FAIL;
	}

    float strWidth = DrawLength(szText);

	if(dwFlags & FT_CENTER)
		x -= strWidth / 2.0f;

    x -= (float)m_chrSpacing;

	if( FAILED(CD3DBaseRender::BeginRender()) )
	{
		_DEBUG("::Print() BeginRender() failed");
		return E_FAIL;
	}

	if( dwFlags & FT_BORDER )
	{
		if( SUCCEEDED(m_pRender->Begin(D3DPT_TRIANGLELIST)) )
		{
			D3DAddQuad(m_pRender, x-1.0f, y-1.0f, strWidth+2.0f, m_fChrHeight+2.0f, 0x3F000000);
			m_pRender->End();
		}		
	}

	DWORD fvf;
	m_pD3Ddev->GetFVF(&fvf);
	m_pD3Ddev->SetFVF(D3DFVF_BITMAPFONT);
	m_pD3Ddev->SetTexture(0, m_pD3Dtex);
	m_pD3Ddev->SetStreamSource(0, m_pD3Dbuf, 0, sizeof(d3dfont_s));

	while(*szText != '\0')
	{
		UINT usedTriangles = 0;
		d3dfont_s *pVertex;

		if( FAILED(m_pD3Dbuf->Lock(0, 0, (void**)&pVertex, D3DLOCK_DISCARD)) )
		{
			_DEBUG("::Print() Lock() failed");
			return E_FAIL;
		}

		for(; *szText != '\0'; szText++)
		{
			char c = (*szText)-32;
			if(!(c >= 0 && c < 96))
				continue;

			float tx1 = m_fTexCoords[c][0];
			float tx2 = m_fTexCoords[c][2];
		    float ty1 = m_fTexCoords[c][1];
			float ty2 = m_fTexCoords[c][3];
			float w = (tx2-tx1) * m_texWidth;
			float h = (ty2-ty1) * m_texHeight;

			*pVertex++ = InitFont2DVertex(x-0.5f,	y-0.5f,		colour, tx1, ty1); //topleft
			*pVertex++ = InitFont2DVertex(x+w-0.5f,	y-0.5f,		colour, tx2, ty1); //topright
			*pVertex++ = InitFont2DVertex(x-0.5f,	y+h-0.5f,	colour, tx1, ty2); //bottomleft
	
			*pVertex++ = InitFont2DVertex(x+w-0.5f,	y-0.5f,		colour, tx2, ty1); //topright
			*pVertex++ = InitFont2DVertex(x+w-0.5f,	y+h-0.5f,	colour, tx2, ty2); //bottomright
			*pVertex++ = InitFont2DVertex(x-0.5f,	y+h-0.5f,	colour, tx1, ty2); //bottomleft
	
			x += w - (m_chrSpacing*2);

			usedTriangles += 2;
			if(usedTriangles >= m_maxTriangles)
				break;
		}

		if(usedTriangles > 0)
		{
			m_pD3Dbuf->Unlock();
			m_pD3Ddev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, usedTriangles);
		}
	}

	m_pD3Ddev->SetFVF(fvf);
	CD3DBaseRender::EndRender();

	return S_OK;
}

float CD3DFont::DrawLength(char *szText) const
{
	float len = 0.0f;

	for(char *p=szText; *p; p++)
	{
		char c = (*p)-32;
		if(c >= 0 && c < 96)
			len += ((m_fTexCoords[c][2]-m_fTexCoords[c][0]) * m_texWidth) - m_chrSpacing*2;
	}

	return len;
}

/*
ABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRS
TUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKL
MNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDE
*/

CD3DRender::CD3DRender(int numVertices)
{
	m_canRender = false;

	m_pD3Dbuf = NULL;
	m_pVertex = NULL;

	m_maxVertex = numVertices;
	m_curVertex = 0;
}


CD3DRender::~CD3DRender()
{
	Invalidate();
}


HRESULT CD3DRender::Initialize(IDirect3DDevice9 *pD3Ddev)
{
	if( !m_canRender )
	{
		if( FAILED(CD3DBaseRender::Initialize(pD3Ddev)) )
			return E_FAIL;
		
		if( FAILED(m_pD3Ddev->CreateVertexBuffer(m_maxVertex * sizeof(d3dprimitives_s), D3DUSAGE_WRITEONLY|D3DUSAGE_DYNAMIC, 0, D3DPOOL_DEFAULT, &m_pD3Dbuf, NULL)) )
			return E_FAIL;
	
		m_canRender = true;
	}

	return S_OK;
}


HRESULT CD3DRender::Invalidate()
{
	CD3DBaseRender::Invalidate();
	SAFE_RELEASE(m_pD3Dbuf);
    m_canRender = false;
	m_pVertex = NULL;

	return S_OK;
}


HRESULT CD3DRender::Begin(D3DPRIMITIVETYPE primType)
{
	if( !m_canRender )
		return E_FAIL;

	if( m_pVertex != NULL )
		return E_FAIL;

	if( FAILED(m_pD3Dbuf->Lock(0, 0, (void**)&m_pVertex, D3DLOCK_DISCARD)) )
		return E_FAIL;
	
	m_primType = primType;

	return S_OK;
}


HRESULT CD3DRender::End()
{
	m_pVertex = NULL;

	if( !m_canRender )
	{
		m_curVertex = 0;
		return E_FAIL;
	}

	if( FAILED(CD3DBaseRender::BeginRender()) )
		return E_FAIL;

	int numPrims;

	switch(m_primType)
	{
		case D3DPT_POINTLIST:
			numPrims = m_curVertex;
			break;
		case D3DPT_LINELIST:
			numPrims = (int)floor(m_curVertex / 2.0f);
			break;
		case D3DPT_LINESTRIP:
			numPrims = m_curVertex - 1;
			break;
		case D3DPT_TRIANGLELIST:
			numPrims = (int)floor(m_curVertex / 3.0f);
			break;
		case D3DPT_TRIANGLESTRIP:
		case D3DPT_TRIANGLEFAN:
			numPrims = m_curVertex - 2;
			break;
		default:
			numPrims = 0;
			break;
	}

	m_curVertex = 0;

	if( numPrims > 0 )
	{
		m_pD3Dbuf->Unlock();

		DWORD fvf;
		m_pD3Ddev->GetFVF(&fvf);
		m_pD3Ddev->SetFVF(D3DFVF_PRIMITIVES);

		m_pD3Ddev->SetTexture(0, NULL);
		m_pD3Ddev->SetStreamSource(0, m_pD3Dbuf, 0, sizeof(d3dprimitives_s));

		m_pD3Ddev->DrawPrimitive(m_primType, 0, numPrims);

		m_pD3Ddev->SetFVF(fvf);
	}

	CD3DBaseRender::EndRender();

	return S_OK;
}


inline HRESULT CD3DRender::D3DColour(DWORD colour)
{
	m_colour = colour;
	return ( m_canRender ? S_OK : E_FAIL );
}


inline HRESULT CD3DRender::D3DVertex2f(float x, float y)
{
	if( m_canRender && m_pVertex && ++m_curVertex < m_maxVertex )
		*m_pVertex++ = InitPrim2DVertex(x, y, m_colour);
	else
		return E_FAIL;

	return S_OK;
}


//helper functions
HRESULT D3DAddQuad(CD3DRender *pRender, float x, float y, float w, float h, DWORD colour)
{
	if( pRender && SUCCEEDED(pRender->D3DColour(colour))
	&& SUCCEEDED(pRender->D3DVertex2f(x, y))		//triangle 1
	&& SUCCEEDED(pRender->D3DVertex2f(x+w, y))
	&& SUCCEEDED(pRender->D3DVertex2f(x, y+h))
	&& SUCCEEDED(pRender->D3DVertex2f(x, y+h))		//triangle 2
	&& SUCCEEDED(pRender->D3DVertex2f(x+w, y))
	&& SUCCEEDED(pRender->D3DVertex2f(x+w, y+h)) )
		return S_OK;

	return E_FAIL;
}
