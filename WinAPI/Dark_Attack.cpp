#include "pch.h"
#include "Dark_Attack.h"
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


Dark_Attack::Dark_Attack()
	: m_Acctime(0.f)
	, m_Duration(0.8f)
	, m_Collider(nullptr)
{


	// 컴포넌트 추가
	m_Collider = AddComponent(new CCollider);
	m_Animator = AddComponent(new CAnimator);

	m_Animator->LoadAnimation(L"animation\\Effect\\Dark_A_Effect_Right.anim");
	m_Animator->LoadAnimation(L"animation\\Effect\\Dark_A_Effect_Left.anim");
	m_Animator->LoadAnimation(L"animation\\Effect\\Dark_B_Effect_Right.anim");
	m_Animator->LoadAnimation(L"animation\\Effect\\Dark_B_Effect_Left.anim");
	m_Animator->LoadAnimation(L"animation\\Effect\\Dark_C_Effect_Right.anim");
	m_Animator->LoadAnimation(L"animation\\Effect\\Dark_C_Effect_Left.anim");
}

Dark_Attack::Dark_Attack(const Dark_Attack& _Other)
	: CObj(_Other)
	, m_Acctime(0.f)
	, m_Animator(nullptr)
	, m_Collider(nullptr)
	, m_Duration(0.f)
{
	m_Collider = (CCollider*)GetComponentByName(L"Player_Attack");
	m_Animator = GetComponent<CAnimator>();
}

Dark_Attack::~Dark_Attack()
{
}



void Dark_Attack::Tick()
{
	m_Acctime += DT;

	if (m_Acctime > m_Duration)
	{
		DeleteObject(this);
	}
}

void Dark_Attack::Render()
{
	m_Animator->Render();
}

void Dark_Attack::BeginOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider)
{
	if (_OtherObj->GetName() == L"Monster")
	{
		Player_Effect* m_Effect = new Player_Effect;
		m_Effect->SetDuration(1.f);
		m_Effect->SetPos(_OtherObj->GetPos());
		CLevelMgr::Get()->GetCurrentLevel()->AddObject(m_Effect, LAYER_TYPE::PLAYER_PROJECTILE);
	}
}

void Dark_Attack::SetCollider(float _X, float _Y)
{
	m_Collider->SetScale(Vec2(_X, _Y));
}