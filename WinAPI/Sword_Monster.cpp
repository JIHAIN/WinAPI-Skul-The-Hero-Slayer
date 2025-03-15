#include "pch.h"
#include "Sword_Monster.h"

#include "CEngine.h"
#include "CCollider.h"
#include "CRigidBody.h"
#include "CTimeMgr.h"
#include "CAnimator.h"
#include "CAssetMgr.h"

#include "CStateMachine.h"
#include "Sword_Idle_State.h"
#include "Sword_Attack_State.h"
#include "Sword_Trace_State.h"
#include "Sword_Hit_State.h"




Sword_Monster::Sword_Monster()
{
	// 콜라이더
	m_Collider = AddComponent(new CCollider);
	m_Collider->SetName(L"Monster_collider");
	m_Collider->SetScale(Vec2(60.f, 90.f));
	m_Collider->SetOffset(Vec2(0.f, 15.f));

	// 리지드바디
	m_RigidBody = AddComponent(new CRigidBody);
	m_RigidBody->SetFriction(100.f);
	m_RigidBody->SetMode(RIGIDBODY_MODE::TOPVIEW);
	m_RigidBody->SetMass(1);
	m_RigidBody->SetMaxGravitySpeed(300.f);

	//몬스터 구조체 채우기
	m_Info.Speed = 100.f;
	m_Info.DetectRange = 400.f;
	m_Info.AttackRange = 100.f;
	m_Info.AtkCoolTime = 1.f;
	m_Info.reconRange = 200.f;
	m_Info.Atk = 20.f;
	m_Info.MaxHP = 100.f;
	m_Info.HP = 100.f;

	//상태머신 추가
	pSM = AddComponent(new CStateMachine);
	pSM->AddState(L"IdleState", new Sword_Idle_State);
	pSM->AddState(L"AttackState", new Sword_Attack_State);
	pSM->AddState(L"TraceState", new Sword_Trace_State);
	pSM->AddState(L"HitState", new Sword_Hit_State);
	pSM->ChangeState(L"IdleState");

	// 애니메이션
	m_Animator = AddComponent(new CAnimator);
	m_Animator->LoadAnimation(L"animation\\Monster\\Sword_Atk_Left.anim");
	m_Animator->LoadAnimation(L"animation\\Monster\\Sword_Atk_Right.anim");
	m_Animator->LoadAnimation(L"animation\\Monster\\Sword_Move_Left.anim");
	m_Animator->LoadAnimation(L"animation\\Monster\\Sword_Move_Right.anim");
	m_Animator->LoadAnimation(L"animation\\Monster\\Sword_Hit_Left.anim");
	m_Animator->LoadAnimation(L"animation\\Monster\\Sword_Hit_Right.anim");

	m_Animator->Play(L"Sword_Move_Left", true);
}

Sword_Monster::Sword_Monster(const Sword_Monster& _Other)
{
}

Sword_Monster::~Sword_Monster()
{
}

void Sword_Monster::BeginOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider)
{
	if (_OtherObj->GetName() == L"Player_Attack")
	{
		m_Info.HP -= 20;

		pSM->ChangeState(L"HitState");

		if (m_Info.HP <= 0)
		{
			DeleteObject(this);
		}
	}
}