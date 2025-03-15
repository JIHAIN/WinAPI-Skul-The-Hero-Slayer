#include "pch.h"
#include "BigEnt_Atk.h"

#include "CAnimator.h"
#include "CAnim.h"
#include "CTimeMgr.h"
#include "CPathMgr.h"
#include "CCollider.h"

BigEnt_Atk::BigEnt_Atk()
{
	m_Collider = AddComponent(new CCollider);
	m_Animator = AddComponent(new CAnimator);

	m_Collider->SetScale(Vec2(600.f, 100.f));
	m_Duration = 1.8f;

	m_Animator->LoadAnimation(L"animation\\Monster\\BigEnt_Atk_Obj.anim");

	m_Animator->Play(L"BigEnt_Atk_Obj", false);
}

BigEnt_Atk::BigEnt_Atk(const BigEnt_Atk& _Other)
{
}

BigEnt_Atk::~BigEnt_Atk()
{
}