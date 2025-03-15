#include "pch.h"
#include "Dash_Effect.h"
#include "CAnimator.h"
#include "CTimeMgr.h"

Dash_Effect::Dash_Effect(bool _Right)
	: m_Animator(nullptr)
	, LifeTime(0.f)
{

	m_Animator = AddComponent(new CAnimator);

	m_Animator->LoadAnimation(L"animation\\Effect\\Dash_Effect_Left.anim");
	m_Animator->LoadAnimation(L"animation\\Effect\\Dash_Effect_Right.anim");

	if (_Right) // 트루라면 오른쪽 재생
	{
		m_Animator->Play(L"Dash_Effect_Right", false);
	}
	else
	{
		m_Animator->Play(L"Dash_Effect_Left", false);
	}

}

Dash_Effect::Dash_Effect(const Dash_Effect& _Other)
	: CObj(_Other)
	, m_Animator(nullptr)
{
	m_Animator = GetComponent<CAnimator>();
}

Dash_Effect::~Dash_Effect()
{
}

void Dash_Effect::Tick()
{
	LifeTime += DT;

	if (LifeTime > 0.6f)
	{
		DeleteObject(this);
	}
}

void Dash_Effect::Render()
{
	if (m_Animator != nullptr)
	{
		m_Animator->Render();
	}
}