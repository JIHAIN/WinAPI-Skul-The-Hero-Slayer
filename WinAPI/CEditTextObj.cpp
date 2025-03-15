#include "pch.h"
#include "CEditTextObj.h"
#include "CTexture.h"
#include "CEngine.h"



CEditTextObj::CEditTextObj()
	: m_Texture(nullptr)
{

}

CEditTextObj::CEditTextObj(const CEditTextObj& _Other)
	: CObj(_Other)
	, m_Texture(_Other.m_Texture)
{

}

CEditTextObj::~CEditTextObj()
{
}

void CEditTextObj::Render()
{

	if (m_Texture == nullptr)
	{
		return;
	}
		
	Vec2 vPos = GetRenderPos();
	UINT width = m_Texture->GetWidth();
	UINT height = m_Texture->GetHeight();

	// AlphaBlending
	{
		BLENDFUNCTION blend = {};
		blend.BlendOp = AC_SRC_OVER;
		blend.BlendFlags = 0;
		blend.SourceConstantAlpha = 255;
		blend.AlphaFormat = AC_SRC_ALPHA;

		AlphaBlend(BackDC, (int)vPos.x, (int)vPos.y
			, width, height, m_Texture->GetDC(), 0, 0, width, height, blend);
	}
}