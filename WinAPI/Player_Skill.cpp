#include "pch.h"
#include "Player_Skill.h"


#include "CTexture.h"
#include "CEngine.h"
#include "CAssetMgr.h"
#include "CCollider.h"

#include "CPlayer.h"
#include "CLevel.h"
#include "CLevelMgr.h"


Player_Skill::Player_Skill()
	: m_PlayerImge(nullptr)
	, m_Player(nullptr)

{
	m_Player = CLevelMgr::Get()->GetCurrentLevel()->GetPlayer();


}

Player_Skill::Player_Skill(const Player_Skill& _Other)
{
}


Player_Skill::~Player_Skill()
{
}


void Player_Skill::Render()
{



	if (m_Player->GetPlayerprom()) // 다크 폼일시
	{
		m_PlayerImge = CAssetMgr::Get()->LoadTexture(L"DarkWave_ICon", L"texture\\DarkWave_ICon.png");
	}
	else // 기본 스컬일시
	{
		m_PlayerImge = CAssetMgr::Get()->LoadTexture(L"Skul_A_Skill", L"texture\\Skul_A_Skill.png");
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