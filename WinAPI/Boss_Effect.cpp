#include "pch.h"
#include "Boss_Effect.h"
#include "CAnimator.h"
#include "CTimeMgr.h"

Boss_Effect::Boss_Effect()
	: m_Animator(nullptr)
	, LifeTime(0.f)
{

	m_Animator = AddComponent(new CAnimator);

	//
	m_Animator->LoadAnimation(L"animation\\Effect\\Boss_Ball_Create.anim");
	m_Animator->LoadAnimation(L"animation\\Effect\\Boss_Ball_End.anim");
	m_Animator->LoadAnimation(L"animation\\Effect\\Boss_Ball_Enter.anim");

	//m_Animator->Play(L"Boss_Stamp_Effect_Left", false);

}

Boss_Effect::Boss_Effect(const Boss_Effect& _Other)
	: CObj(_Other)
	, m_Animator(nullptr)
{
	m_Animator = GetComponent<CAnimator>();
}

Boss_Effect::~Boss_Effect()
{
}

void Boss_Effect::Tick()
{
	LifeTime += DT;

	if (LifeTime > 6.0f)
	{
		DeleteObject(this);
	}
}

void Boss_Effect::Render()
{
	if (m_Animator != nullptr)
	{
		m_Animator->Render();
	}
}