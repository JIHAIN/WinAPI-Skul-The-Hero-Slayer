#include "pch.h"
#include "Player_Skill2.h"

#include "CTexture.h"
#include "CEngine.h"
#include "CAssetMgr.h"
#include "CCollider.h"

#include "CPlayer.h"
#include "CLevel.h"
#include "CLevelMgr.h"
#include "CAnimator.h"

#include "CKeyMgr.h"


Player_Skill2::Player_Skill2()
	: m_PlayerImge(nullptr)
	, m_Player(nullptr)
{
	m_Player = CLevelMgr::Get()->GetCurrentLevel()->GetPlayer();



}

Player_Skill2::Player_Skill2(const Player_Skill2& _Other)
{
}


Player_Skill2::~Player_Skill2()
{
}


void Player_Skill2::Render()
{

	if (m_Player->GetPlayerprom()) // 다크 폼일시
	{
		m_PlayerImge = CAssetMgr::Get()->LoadTexture(L"DarkPaladin_Swap", L"texture\\DarkPaladin_Swap.png");
	}
	else // 기본 스컬일시
	{
		m_PlayerImge = CAssetMgr::Get()->LoadTexture(L"Skul_S_Skill", L"texture\\Skul_S_Skill.png");
	}

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