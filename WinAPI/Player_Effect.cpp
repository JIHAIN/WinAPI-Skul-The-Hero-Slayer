#include "pch.h"
#include "Player_Effect.h"
#include "CAnimator.h"
#include "CTimeMgr.h"
#include "CPlayer.h"
#include "CLevelMgr.h"
#include "CLevel.h"

Player_Effect::Player_Effect()
	: m_Animator(nullptr)
	, LifeTime(0.f)
	, Duration(0.f)
	, m_Player(nullptr)
{

	m_Animator = AddComponent(new CAnimator);

	m_Animator->LoadAnimation(L"animation\\Effect\\Hit_Cirtical.anim");
	m_Animator->LoadAnimation(L"animation\\Effect\\Normal_Hit.anim");
	
	m_Player = CLevelMgr::Get()->GetCurrentLevel()->GetPlayer();

	

}

Player_Effect::Player_Effect(const Player_Effect& _Other)
	: CObj(_Other)
	, m_Animator(nullptr)
{
	m_Animator = GetComponent<CAnimator>();
}

Player_Effect::~Player_Effect()
{
}

void Player_Effect::Tick()
{
	LifeTime += DT;

	if (LifeTime > Duration)
	{
		DeleteObject(this);
	}
}

void Player_Effect::Render()
{
	if (m_Player->GetPlayerprom()) // 다크 폼일시
	{
		m_Animator->Play(L"Hit_Cirtical", false);
	}
	else // 기본 스컬일시
	{
		m_Animator->Play(L"Normal_Hit", false);
	}

	if (m_Animator != nullptr)
	{
		m_Animator->Render();
	}
}