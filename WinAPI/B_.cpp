#include "pch.h"
#include "B_.h"
#include "CTexture.h"
#include "CEngine.h"
#include "CAssetMgr.h"
#include "CCollider.h"

#include "CPlayer.h"
#include "CLevel.h"
#include "CLevelMgr.h"


B_::B_()
	: m_PlayerImge(nullptr)
{
	m_PlayerImge = CAssetMgr::Get()->LoadTexture(L"S_", L"texture\\S_.png");
}

B_::B_(const B_& _Other)
{
}


B_::~B_()
{
}


void B_::Render()
{
	if (m_PlayerImge == nullptr)
	{
		return;
	}

	Vec2 vPos = GetPos();
	UINT width = m_PlayerImge->GetWidth();
	UINT height = m_PlayerImge->GetHeight();

	// AlphaBlending
	BLENDFUNCTION blend = {};
	blend.BlendOp = AC_SRC_OVER;
	blend.BlendFlags = 0;
	blend.SourceConstantAlpha = 255;
	blend.AlphaFormat = AC_SRC_ALPHA;

	AlphaBlend(BackDC, (int)vPos.x, (int)vPos.y
		, width, height, m_PlayerImge->GetDC(), 0, 0, width, height, blend);

}