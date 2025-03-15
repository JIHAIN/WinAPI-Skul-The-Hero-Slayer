#include "pch.h"
#include "Effect_Obj.h"
#include "CAnimator.h"
#include "CTimeMgr.h"

Effect_Obj::Effect_Obj()
	: m_Animator(nullptr)
	, LifeTime(0.f)
	, Duration(0.f)
{

	m_Animator = AddComponent(new CAnimator);

	m_Animator->LoadAnimation(L"animation\\Effect\\Boss_Stamp_Effect_Left.anim");
	m_Animator->LoadAnimation(L"animation\\Effect\\Snap_Effect_Left.anim");
	m_Animator->LoadAnimation(L"animation\\Effect\\Snap_Effect_Right.anim");
	
	//m_Animator->Play(L"Boss_Stamp_Effect_Left", false);

}

Effect_Obj::Effect_Obj(const Effect_Obj& _Other)
	: CObj(_Other)
	, m_Animator(nullptr)
{
	m_Animator = GetComponent<CAnimator>();
}

Effect_Obj::~Effect_Obj()
{
}

void Effect_Obj::Tick()
{
	LifeTime += DT;

	if (LifeTime > Duration)
	{
		DeleteObject(this);
	}
}

void Effect_Obj::Render()
{
	if (m_Animator != nullptr)
	{
		m_Animator->Render();
	}
}