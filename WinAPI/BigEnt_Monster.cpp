#include "pch.h"
#include "BigEnt_Monster.h"

#include "CEngine.h"
#include "CCollider.h"
#include "CRigidBody.h"
#include "CTimeMgr.h"
#include "CAnimator.h"
#include "CAssetMgr.h"

#include "CStateMachine.h"
#include "BigEnt_Idle_State.h"
#include "BigEnt_Attack_State.h"
#include "BigEnt_Trace_State.h"
#include "BigEnt_Hit_State.h"




BigEnt_Monster::BigEnt_Monster()
{
	m_Collider = AddComponent(new CCollider);
	m_Collider->SetName(L"Monster_collider");
	m_Collider->SetScale(Vec2(200.f, 200.f));

	//상태머신 추가
	CStateMachine* pSM = AddComponent(new CStateMachine);

	pSM->AddState(L"IdleState", new BigEnt_Idle_State);
	pSM->AddState(L"AttackState", new BigEnt_Attack_State);
	pSM->AddState(L"TraceState", new BigEnt_Trace_State);
	pSM->AddState(L"HitState", new BigEnt_Hit_State);
	pSM->ChangeState(L"IdleState");

	m_RigidBody = AddComponent(new CRigidBody);
	m_RigidBody->SetFriction(100.f);
	m_RigidBody->SetMode(RIGIDBODY_MODE::TOPVIEW);
	m_RigidBody->SetMass(1);
	m_RigidBody->SetMaxGravitySpeed(300.f);

	m_Info.Speed = 100.f;
	m_Info.DetectRange = 500.f;
	m_Info.AttackRange = 300.f;
	m_Info.AtkCoolTime = 2.f;
	m_Info.Atk = 20.f;
	m_Info.MaxHP = 100.f;
	m_Info.HP = 100.f;

	m_Animator = AddComponent(new CAnimator);
	m_Animator->LoadAnimation(L"animation\\Monster\\BigEnt_IDLE.anim");
	m_Animator->LoadAnimation(L"animation\\Monster\\BigEnt_LOW.anim");
	m_Animator->LoadAnimation(L"animation\\Monster\\BigEnt_Atk.anim");

	m_Animator->Play(L"BigEnt_IDLE", true);
}

BigEnt_Monster::BigEnt_Monster(const BigEnt_Monster& _Other)
{
}

BigEnt_Monster::~BigEnt_Monster()
{
}

