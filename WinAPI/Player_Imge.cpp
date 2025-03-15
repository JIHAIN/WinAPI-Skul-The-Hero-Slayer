#include "pch.h"
#include "Player_Imge.h"

#include "CTexture.h"
#include "CEngine.h"
#include "CAssetMgr.h"
#include "CCollider.h"

#include "CPlayer.h"
#include "CLevel.h"
#include "CLevelMgr.h"


Player_Imge::Player_Imge()
	: m_PlayerImge(nullptr)
	, m_Player(nullptr)
{
	m_Player = CLevelMgr::Get()->GetCurrentLevel()->GetPlayer();


	m_PlayerImge = CAssetMgr::Get()->LoadTexture(L"Skul_Imge", L"texture\\Skul_Super.png");
}

Player_Imge::Player_Imge(const Player_Imge& _Other)
{
}


Player_Imge::~Player_Imge()
{
}


void Player_Imge::Render()
{
	if (m_Player->GetPlayerprom()) // 다크 폼일시
	{
		m_PlayerImge = CAssetMgr::Get()->LoadTexture(L"Player_Dark_porm", L"texture\\Player_Dark_porm.png");
	}
	else // 기본 스컬일시
	{
		m_PlayerImge = CAssetMgr::Get()->LoadTexture(L"Skul_Imge", L"texture\\Skul_Super.png");
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

	AlphaBlend(BackDC, (int)vPos.x - 12.f, (int)vPos.y - 8.f
		, width, height, m_PlayerImge->GetDC(), 0, 0, width, height, blend);

}