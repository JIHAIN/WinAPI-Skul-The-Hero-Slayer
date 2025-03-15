#include "pch.h"
#include "Skil_Col.h"
#include "CAnimator.h"
#include "CTimeMgr.h"

Skil_Col::Skil_Col()
	: m_Animator(nullptr)
	, AccTime(0.f)
{
	m_Animator = AddComponent(new CAnimator);

	m_Animator->LoadAnimation(L"animation\\Effect\\Skill_Col.anim");

	m_Animator->Play(L"Skill_Col", false);

}

Skil_Col::Skil_Col(const Skil_Col& _Other)
{
}

Skil_Col::~Skil_Col()
{
}

void Skil_Col::Render()
{
	m_Animator->UI_Render();
}

void Skil_Col::Tick()
{
	AccTime += DT;

	if (AccTime > 1.2f)
	{
		DeleteObject(this);
	}
}


