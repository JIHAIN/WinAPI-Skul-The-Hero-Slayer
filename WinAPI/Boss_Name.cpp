#include "pch.h"
#include "Boss_Name.h"
#include "CTexture.h"
#include "CEngine.h"
#include "CAssetMgr.h"
#include "CCollider.h"

#include "CLevel.h"
#include "CLevelMgr.h"
#include "Boss_HP.h"
#include "Boss_Monster.h"



Boss_Name::Boss_Name(Vec2 _Pos)
	: m_Texture(nullptr)
	, m_BossHpBar(nullptr)
	, m_Boss(nullptr)
{
	m_Boss = CLevelMgr::Get()->GetCurrentLevel()->GetBoss();
	m_Texture = CAssetMgr::Get()->LoadTexture(L"Boss_namebar", L"texture\\Boss_namebar.png");

	CLevel* CurLevel = CLevelMgr::Get()->GetCurrentLevel();

	m_BossHpBar = new Boss_HP;
	m_BossHpBar->SetPos(_Pos);
	CurLevel->AddObject(m_BossHpBar, LAYER_TYPE::MYUIUP);

}

Boss_Name::Boss_Name(const Boss_Name& _Other)
{
}


Boss_Name::~Boss_Name()
{
}

void Boss_Name::Tick()
{
	if (m_Boss->IsDead() || m_Boss == nullptr)
	{
		DeleteObject(this);
	}
}

void Boss_Name::Render()
{

	if (m_Texture == nullptr)
	{
		return;
	}

	Vec2 vPos = GetPos();
	UINT width = m_Texture->GetWidth();
	UINT height = m_Texture->GetHeight();

	// AlphaBlending
	BLENDFUNCTION blend = {};
	blend.BlendOp = AC_SRC_OVER;
	blend.BlendFlags = 0;
	blend.SourceConstantAlpha = 255;
	blend.AlphaFormat = AC_SRC_ALPHA;

	AlphaBlend(BackDC, (int)vPos.x, (int)vPos.y
		, width, height, m_Texture->GetDC(), 0, 0, width, height, blend);

}