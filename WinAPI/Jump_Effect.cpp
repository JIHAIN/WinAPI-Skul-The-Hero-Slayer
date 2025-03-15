#include "pch.h"
#include "Jump_Effect.h"
#include "CAnimator.h"
#include "CTimeMgr.h"

Jump_Effect::Jump_Effect()
	: m_Animator(nullptr)
	, LifeTime(0.f)
{

	m_Animator = AddComponent(new CAnimator);

	m_Animator->LoadAnimation(L"animation\\Effect\\Jump_Effect.anim");

	m_Animator->Play(L"Jump_Effect", false);

}

Jump_Effect::Jump_Effect(const Jump_Effect& _Other)
	: CObj(_Other)
	, m_Animator(nullptr)
{
	m_Animator = GetComponent<CAnimator>();
}

Jump_Effect::~Jump_Effect()
{
}

void Jump_Effect::Tick()
{
	LifeTime += DT;

	if (LifeTime > 0.6f)
	{
		DeleteObject(this);
	}
}

void Jump_Effect::Render()
{
	if (m_Animator != nullptr)
	{
		m_Animator->Render();
	}
}