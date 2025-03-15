#include "pch.h"
#include "Sniper_Monster.h"

#include "CEngine.h"
#include "CCollider.h"
#include "CRigidBody.h"
#include "CTimeMgr.h"
#include "CAnimator.h"
#include "CAssetMgr.h"

#include "CStateMachine.h"
#include "Sniper_Idle_State.h"
#include "Sniper_Attack_State.h"
#include "Sniper_Trace_State.h"
#include "Sniper_Hit_State.h"



Sniper_Monster::Sniper_Monster()
{
	// �ݶ��̴�
	m_Collider = AddComponent(new CCollider);
	m_Collider->SetName(L"Monster_collider");
	m_Collider->SetScale(Vec2(60.f, 90.f));

	// ������ٵ�
	m_RigidBody = AddComponent(new CRigidBody);
	m_RigidBody->SetFriction(100.f);
	m_RigidBody->SetMode(RIGIDBODY_MODE::TOPVIEW);
	m_RigidBody->SetMass(1);
	m_RigidBody->SetMaxGravitySpeed(300.f);

	//���� ����ü ä���
	m_Info.Speed = 100.f;
	m_Info.DetectRange = 500.f;
	m_Info.AttackRange = 300.f;
	m_Info.AtkCoolTime = 2.f;
	m_Info.reconRange = 200.f;
	m_Info.Atk = 20.f;
	m_Info.MaxHP = 100.f;
	m_Info.HP = 100.f;

	//���¸ӽ� �߰�
	pSM = AddComponent(new CStateMachine);
	pSM->AddState(L"IdleState", new Sniper_Idle_State);
	pSM->AddState(L"AttackState", new Sniper_Attack_State);
	pSM->AddState(L"TraceState", new Sniper_Trace_State);
	pSM->AddState(L"HitState", new Sniper_Hit_State);
	pSM->ChangeState(L"IdleState");

	// �ִϸ��̼�
	m_Animator = AddComponent(new CAnimator);
	m_Animator->LoadAnimation(L"animation\\Monster\\Sniper_aiming_Right.anim");
	m_Animator->LoadAnimation(L"animation\\Monster\\Sniper_aiming_Left.anim");
	m_Animator->LoadAnimation(L"animation\\Monster\\Sniper_Move_Left.anim");
	m_Animator->LoadAnimation(L"animation\\Monster\\Sniper_Move_Right.anim");
	m_Animator->LoadAnimation(L"animation\\Monster\\Sniper_Hit_Left.anim");
	m_Animator->LoadAnimation(L"animation\\Monster\\Sniper_Hit_Right.anim");

	m_Animator->Play(L"Sniper_Move_Left", true);
}

Sniper_Monster::Sniper_Monster(const Sniper_Monster& _Other)
{
}

Sniper_Monster::~Sniper_Monster()
{
}

void Sniper_Monster::BeginOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider)
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