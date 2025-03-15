#include "pch.h"
#include "Boss_Monster.h"

#include "CEngine.h"
#include "CCollider.h"
#include "CRigidBody.h"
#include "CTimeMgr.h"
#include "CAnimator.h"
#include "CAssetMgr.h"

#include "BossLeft.h"
#include "BossRight.h"
#include "Boss_Body.h"
#include "Boss_Chin.h"

#include "CLevel.h"
#include "CLevelMgr.h"

#include "END_Level.h"



// 상태머신들
#include "Boss_Idle.h"
#include "Snap_End_State.h"
#include "Snaping_State.h"
#include "Snap_Enter_State.h"
#include "Stamp_End_State.h"
#include "Stamp_Enter_State.h"
#include "Stamping_State.h"
#include "Balling_State.h"
#include "Ball_Enter_State.h"





Boss_Monster::Boss_Monster()
	: m_TargetObj(nullptr)
	, m_Info{}
	, m_Animator(nullptr)
	, m_Collider(nullptr)
	, m_RigidBody(nullptr)
	, Hand_Left(nullptr)
	, Hand_Right(nullptr)
	, Body(nullptr)
	, Chin(nullptr)
	, Boss_Atk_Cheak(false)
	, Boss_flor_Cheak(true)
{

}

// 위치정보 받는 생성자 버전
Boss_Monster::Boss_Monster(Vec2 _Pos)
	: m_TargetObj(nullptr)
	, m_Info{}
	, m_Animator(nullptr)
	, m_Collider(nullptr)
	, m_RigidBody(nullptr)
	, Hand_Left(nullptr)
	, Hand_Right(nullptr)
	, Body(nullptr)
	, Chin(nullptr)
	, phase2(false)
{
	CLevel* CurLevel = CLevelMgr::Get()->GetCurrentLevel();

	// 애니메이션

	m_Animator = AddComponent(new CAnimator);
	m_Animator->LoadAnimation(L"animation\\Boss\\Boss_Head1_Idle.anim");
	m_Animator->LoadAnimation(L"animation\\Boss\\Boss_Head1_Atk.anim");
	m_Animator->LoadAnimation(L"animation\\Boss\\Boss_Head2_Idle.anim");

	m_Animator->Play(L"Boss_Head1_Idle", true);

	Hand_Left = new BossLeft;
	Hand_Left->SetPos(_Pos.x - 350.f, _Pos.y + 130.f);
	Hand_Left->SetInitLeftPos(_Pos.x - 350.f, _Pos.y + 130.f);
	CurLevel->AddObject(Hand_Left, LAYER_TYPE::MONSTER);

	Hand_Right = new BossRight;
	Hand_Right->SetPos(_Pos.x + 350.f, _Pos.y + 130.f);
	Hand_Right->SetInitRightPos(_Pos.x + 350.f, _Pos.y + 130.f);
	CurLevel->AddObject(Hand_Right, LAYER_TYPE::MONSTER);

	Chin = new Boss_Chin;
	Chin->SetPos(_Pos.x, _Pos.y + 60.f);
	CurLevel->AddObject(Chin, LAYER_TYPE::MONSTER);

	Body = new Boss_Body;
	Body->SetPos(_Pos.x, _Pos.y);
	CurLevel->AddObject(Body, LAYER_TYPE::MONSTER);

	// 콜라이더
	m_Collider = AddComponent(new CCollider);
	m_Collider->SetName(L"Monster_collider");
	m_Collider->SetScale(Vec2(260.f, 300.f));
	m_Collider->SetOffset(Vec2(0.f, 50.f));

	// 리지드바디
	m_RigidBody = AddComponent(new CRigidBody);
	m_RigidBody->SetFriction(100.f);
	m_RigidBody->SetMode(RIGIDBODY_MODE::TOPVIEW);
	m_RigidBody->SetMass(1);
	m_RigidBody->SetMaxGravitySpeed(300.f);

	//몬스터 구조체 채우기
	m_Info.Speed = 100.f;
	m_Info.DetectRange = 600.f;
	m_Info.AttackRange = 100.f;
	m_Info.AtkCoolTime = 6.f;
	m_Info.MaxHP = 400.f;
	m_Info.HP = 400.f;
	m_Info.Atk = 30.f;

	//상태머신 추가
	CStateMachine* pSM = AddComponent(new CStateMachine);
	pSM->AddState(L"IdleState", new Boss_Idle);
	pSM->AddState(L"Snap_End", new Snap_End_State);
	pSM->AddState(L"Snaping", new Snaping_State);
	pSM->AddState(L"Snap_Enter", new Snap_Enter_State);
	pSM->AddState(L"Stamp_End", new Stamp_End_State);
	pSM->AddState(L"Stamp_Enter", new Stamp_Enter_State);
	pSM->AddState(L"Stamping", new Stamping_State);
	pSM->AddState(L"Balling_State", new Balling_State);
	pSM->AddState(L"Ball_Enter_State", new Ball_Enter_State);


	pSM->ChangeState(L"IdleState");
}

Boss_Monster::Boss_Monster(const Boss_Monster& _Other)
	: CObj(_Other)
	, m_Animator(nullptr)
	, m_Collider(nullptr)
	, m_RigidBody(nullptr)
	, m_TargetObj(nullptr)
	, m_Info(_Other.m_Info)
{
	// 각 컴포넌트들 새로 만들어주기
	m_Collider = (CCollider*)GetComponentByName(L"Boss_collider");

	m_Animator = GetComponent<CAnimator>();

	m_RigidBody = GetComponent<CRigidBody>();

}

Boss_Monster::~Boss_Monster()
{

}



void Boss_Monster::SetScale(float _Width, float _Height)
{
	if (m_Collider)
	{
		m_Collider->SetScale(Vec2(_Width, _Height));
	}

}

void Boss_Monster::Tick()
{

}

void Boss_Monster::Render()
{
	Body->Render_Part();
	Hand_Left->Render_Part();
	Hand_Right->Render_Part();
	m_Animator->Render();
	Chin->Render_Part();
}

void Boss_Monster::BeginOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider)
{

	if (_OtherObj->GetName() == L"Player_Attack")
	{
		m_Info.HP -= 20;

		if (m_Info.HP <= 0)
		{
			//DeleteObject(this);

			CCamera::Get()->SetLookAt(Vec2(0.f, 0.f));
			CCamera::Get()->SetTargetPos(Vec2(0.f, 0.f));
			ChangeLevel(LEVEL_TYPE::END_Level);
		}
	}

}

void Boss_Monster::Left_Atk_On(bool _Atk)
{
	Hand_Left->LeftAtkOn(_Atk);
}

void Boss_Monster::Right_Atk_On(bool _Atk)
{
	Hand_Right->RightAtkOn(_Atk);
}

void Boss_Monster::flor_On(bool _flor)
{
	Body->Body_flor(_flor);
}



void Boss_Monster::GetPart(Boss_Part& _Part)
{
	_Part.Body = GetBody();
	_Part.Chin = GetChin();
	_Part.Hand_Left = GetLeftHand();
	_Part.Hand_Right = GetRightHand();
}