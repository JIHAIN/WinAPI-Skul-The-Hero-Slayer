#include "pch.h"
#include "CBackground.h"

#include "CEngine.h"
#include "CTexture.h"

CBackground::CBackground()
	: m_Texture(nullptr)
	, IDX(0)
{

	if (m_Texture != nullptr)
		m_Texture = nullptr;


}

CBackground::~CBackground()
{
}


void CBackground::Tick()
{
}

void CBackground::Render()
{

	if (m_Texture == nullptr)
	{
		return;
	}

	Vec2 vPos = GetRenderPos();
	UINT width = m_Texture->GetWidth();
	UINT height = m_Texture->GetHeight();

	// AlphaBlending
	BLENDFUNCTION blend = {};
	blend.BlendOp = AC_SRC_OVER;
	blend.BlendFlags = 0;
	blend.SourceConstantAlpha = 255;
	blend.AlphaFormat = AC_SRC_ALPHA;

	//AlphaBlend(BackDC, (int)vPos.x - width / 2, (int)vPos.y - 400 - height /2
	//	, width, height, m_Texture->GetDC(), 0, 0, width, height, blend);

	AlphaBlend(BackDC, (int)vPos.x - width / 2, (int)vPos.y - height / 2
		, width, height, m_Texture->GetDC(), 0, 0, width, height, blend);
}

