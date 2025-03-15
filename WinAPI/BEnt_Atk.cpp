#include "pch.h"
#include "BEnt_Atk.h"

#include "CAnimator.h"
#include "CAnim.h"
#include "CTimeMgr.h"
#include "CPathMgr.h"
#include "CCollider.h"

BEnt_Atk::BEnt_Atk()
{
	m_Collider = AddComponent(new CCollider);
	m_Animator = AddComponent(new CAnimator);

	m_Collider->SetScale(Vec2(200.f, 150.f));
	m_Duration = 2.7f;

	m_Animator->LoadAnimation(L"animation\\Monster\\BEnt_ATK.anim");

	m_Animator->Play(L"BEnt_ATK", false);
}

BEnt_Atk::BEnt_Atk(const BEnt_Atk& _Other)
{
}

BEnt_Atk::~BEnt_Atk()
{
}
