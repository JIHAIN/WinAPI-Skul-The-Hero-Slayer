#include "pch.h"
#include "Ent_Atk.h"

#include "CAnimator.h"
#include "CAnim.h"
#include "CTimeMgr.h"
#include "CPathMgr.h"
#include "CCollider.h"

Ent_Atk::Ent_Atk()
{
	m_Collider = AddComponent(new CCollider);
	m_Animator = AddComponent(new CAnimator);

	m_Collider->SetScale(Vec2(110.f, 90.f));
	m_Duration = 1.5f;

	m_Animator->LoadAnimation(L"animation\\Monster\\Ent_Atk_Obj.anim");

	m_Animator->Play(L"Ent_Atk_Obj", false);
}

Ent_Atk::Ent_Atk(const Ent_Atk& _Other)
{
}

Ent_Atk::~Ent_Atk()
{
}
