#include "pch.h"
#include "CAttack.h"

#include "CEngine.h"
#include "CAssetMgr.h"

#include "CCollider.h"

#include "CAnimator.h"
#include "CAnim.h"
#include "CTimeMgr.h"
#include "CPathMgr.h"



CAttack::CAttack()
	: m_Acctime(0.f)
	, m_Duration(0.4f)
	, m_Collider(nullptr)
{
	// 컴포넌트 추가
	m_Collider = AddComponent(new CCollider);
	m_Animator = AddComponent(new CAnimator);

}

CAttack::CAttack(const CAttack& _Other)
	: CObj(_Other)
	, m_Acctime(0.f)
	, m_Animator(nullptr)
	, m_Collider(nullptr)
	, m_Duration(0.f)
{
	m_Collider = (CCollider*)GetComponentByName(L"Player_Attack");
	m_Animator = GetComponent<CAnimator>();
}

CAttack::~CAttack()
{
}



void CAttack::Tick()
{
	m_Acctime += DT;

	if (m_Acctime > m_Duration)
	{
 		DeleteObject(this);
	}
}

void CAttack::Render()
{
	m_Animator->Render();
}

void CAttack::BeginOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider)
{
}

void CAttack::SetCollider(float _X, float _Y)
{
	m_Collider->SetScale(Vec2(_X, _Y));
}