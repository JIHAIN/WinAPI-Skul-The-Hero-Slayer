#include "pch.h"
#include "Dark_Back.h"
#include "CAnimator.h"
#include "CCollider.h"
#include "CTimeMgr.h"

#include "CPlayer.h"
#include "CLevel.h"
#include "CLevelMgr.h"

Dark_Back::Dark_Back()
	: m_Acctime(0.f)
	, m_Duration(2.f)
	, m_Animator(nullptr)
	, m_Player(nullptr)
{
	m_Player = CLevelMgr::Get()->GetCurrentLevel()->GetPlayer();

	m_Animator = AddComponent(new CAnimator);

	m_Animator->LoadAnimation(L"animation\\Effect\\Dark_Skill_2.anim");
	m_Animator->LoadAnimation(L"animation\\Effect\\Dark_Skill_ing.anim");

	m_Animator->Play(L"Dark_Skill_2", false);
}

Dark_Back::Dark_Back(const Dark_Back& _Other)
	: CObj(_Other)
	, m_Acctime(0.f)
	, m_Animator(nullptr)
	, m_Duration(0.f)
{
	m_Animator = GetComponent<CAnimator>();
}

Dark_Back::~Dark_Back()
{
}



void Dark_Back::Tick()
{
	if (!m_Player->GetPlayerprom())
	{
		DeleteObject(this);
	}

	Vec2 PlaPos = m_Player->GetPos();

	bool Righttrue = m_Player->GetRightPrd();


	if (Righttrue)
	{
		SetPos(PlaPos.x - 50.f, PlaPos.y - 100.f);
	}
	else
	{
		SetPos(PlaPos.x + 50.f, PlaPos.y - 100.f);
	}


	m_Acctime += DT;

	if (m_Acctime > 1.f && m_Duration == 2.f)
	{
		m_Animator->Play(L"Dark_Skill_ing", true);
	}



}

void Dark_Back::Render()
{
	m_Animator->Render();
}

void Dark_Back::BeginOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider)
{
}

