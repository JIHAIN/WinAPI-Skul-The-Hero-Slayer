#include "pch.h"
#include "CPlayer_Normal_Attack.h"

#include "CEngine.h"
#include "CAssetMgr.h"

#include "CCollider.h"

#include "CAnimator.h"
#include "CAnim.h"
#include "CTimeMgr.h"
#include "CPathMgr.h"
#include "Player_Effect.h"
#include "CLevel.h"
#include "CLevelMgr.h"


CPlayer_Normal_Attack::CPlayer_Normal_Attack()
	: m_Collider(nullptr)
	, m_Acctime(0.f)
	, m_Duration(0.1)
{
	// 컴포넌트 추가
	m_Collider = AddComponent(new CCollider);
	m_Animator = AddComponent(new CAnimator);

	
}

CPlayer_Normal_Attack::~CPlayer_Normal_Attack()
{
}

void CPlayer_Normal_Attack::Tick()
{
	m_Acctime += DT;

	if (m_Acctime > m_Duration)
	{
		DeleteObject(this);
	}
}

void CPlayer_Normal_Attack::Render()
{
}

void CPlayer_Normal_Attack::BeginOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider)
{
	if (_OtherObj->GetName() == L"Monster")
	{
		Player_Effect* m_Effect = new Player_Effect;
		m_Effect->SetDuration(1.f);
		m_Effect->SetPos(_OtherObj->GetPos());
		CLevelMgr::Get()->GetCurrentLevel()->AddObject(m_Effect, LAYER_TYPE::PLAYER_PROJECTILE);
	}
}

void CPlayer_Normal_Attack::SetCollider(float _X, float _Y)
{
	m_Collider->SetScale(Vec2(_X, _Y));
}