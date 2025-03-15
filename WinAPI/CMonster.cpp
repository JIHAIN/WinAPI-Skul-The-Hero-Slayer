#include "pch.h"
#include "CMonster.h"

#include "CEngine.h"
#include "CCollider.h"
#include "CRigidBody.h"
#include "CTimeMgr.h"
#include "CAnimator.h"
#include "CAssetMgr.h"




CMonster::CMonster()
	: m_HP(5)
	, m_Speed(100.f)
	, m_DetectRange(300.f)
	, m_TargetObj(nullptr)
	, m_Info{}
	, m_Animator(nullptr)
	, m_Collider(nullptr)
	, m_RigidBody(nullptr)
	, Wall(false)
	, m_PrevPos(0.f,0.f)
{
	// RigidBody
	m_RigidBody = AddComponent(new CRigidBody);
	m_RigidBody->SetMode(RIGIDBODY_MODE::PLATFOMER);
	m_RigidBody->SetMass(1.f);
	m_RigidBody->SetMaxSpeed(1000.f);
	m_RigidBody->SetMaxGravitySpeed(1200.f);
	m_RigidBody->SetGround(false);
}

CMonster::CMonster(const CMonster& _Other)
	: CObj(_Other)
	, m_Animator(nullptr)
	, m_Collider(nullptr)
	, m_RigidBody(nullptr)
	, m_TargetObj(nullptr)
	, m_DetectRange(_Other.m_DetectRange)
	, m_HP(_Other.m_HP)
	, m_Speed(_Other.m_Speed)
	, m_Info(_Other.m_Info)
{
	// 각 컴포넌트들 새로 만들어주기
	m_Collider = (CCollider*)GetComponentByName(L"Monster_collider");
	m_Animator = GetComponent<CAnimator>();
	m_RigidBody = GetComponent<CRigidBody>();

}

CMonster::~CMonster()
{
}



void CMonster::SetScale(float _Width, float _Height)
{
	if (m_Collider)
	{
		m_Collider->SetScale(Vec2(_Width, _Height));
	}

}

void CMonster::Tick()
{
	m_PrevPos = GetPos();


}

void CMonster::Render()
{
	m_Animator->Render();

}

void CMonster::BeginOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider)
{
	if (_OtherObj->GetName() == L"Player_Attack")
	{
		m_Info.HP -= 20.f;

		if (m_Info.HP <= 0)
		{
			DeleteObject(this);
		}
	}
}

