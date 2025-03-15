#include "pch.h"
#include "CKiMissile.h"

#include "CLevelMgr.h"
#include "CLevel.h"
#include "CMonster.h"
#include "CAttack.h"
#include "CAnimator.h"
#include "CAssetMgr.h"

#include "CTimeMgr.h"
#include "CPlayer.h"

CKiMissile::CKiMissile()
	: m_TargetObj(nullptr)
	, m_Animator(nullptr)
	, m_RotateSpeed(300.f)
	, m_AccTime(0.f)
{
	m_Animator = AddComponent(new CAnimator);


	//tAnimDesc desc = {};
	//
	//desc.pAtlas = CAssetMgr::Get()->LoadTexture(L"fire", L"texture\\fire.bmp");
	//desc.AnimName = L"fire";
	//desc.StartLeftTop = Vec2(0.f, 0.f);
	//desc.FrmCount = 6;
	//desc.SliceSize = Vec2(100.f, 62.f);
	//desc.FPS = 10;
	//m_Animator->CreateAnimation(desc);
	//
	//m_Animator->SaveAnimation(L"animation\\Player_Attack\\");

	m_Animator->LoadAnimation(L"animation\\Player_Attack\\fire.anim");
	

	m_Animator->Play(L"fire", true);
}

CKiMissile::CKiMissile(const CKiMissile& _Other)
	: CMissile(_Other)
	, m_AccTime(0.f)
	, m_Animator(nullptr)
	, m_RotateSpeed(_Other.m_RotateSpeed)
	, m_TargetObj(nullptr)
{
	m_Animator = GetComponent<CAnimator>();
}

CKiMissile::~CKiMissile()
{

}

void CKiMissile::Tick()
{
	m_AccTime += DT;
	Vec2 Pos = GetPos();

	Pos += Vec2(-50.f * DT, -50.f * DT);

	SetPos(Pos);

	if (m_AccTime >= 1.f)
	{
		FindTarget();
		m_AccTime = 0;
	}

	if (IsValid(m_TargetObj))
	{
		Trace();
		CMissile::Tick();
	}


}

void CKiMissile::Render()
{
	m_Animator->Render();
}

void CKiMissile::FindTarget()
{
	if (nullptr != m_TargetObj)
		return;

		// 탐지범위
		float MinDist = 1000.f;

	// 미사일의 위치
	Vec2 vMissilePos = GetPos();

	// 미사일(본인)과 가장 가까운 몬스터를 찾는다.
	CLevel* pCurLevel = CLevelMgr::Get()->GetCurrentLevel();
	const vector<CObj*>& vecMon = pCurLevel->GetLayer(LAYER_TYPE::MONSTER);

	for (size_t i = 0; i < vecMon.size(); ++i)
	{
		//Vec2 vDiff = vecMon[i]->GetPos() - vMissilePos;
		//float Dist = vDiff.Length();

		float Dist = vMissilePos.Distance(vecMon[i]->GetPos());

		if (Dist < MinDist)
		{
			MinDist = Dist;
			m_TargetObj = vecMon[i];
		}
	}


}

void CKiMissile::Trace()
{
	Vec2 vDir = m_TargetObj->GetPos() - GetPos();
	vDir.Normalize();
	Vec2 vForce = vDir * 2000.f;
	Vec2 Accel = vForce / GetMass();
	AddVelocity(Accel * DT);


	{
		// 현재 이동중인 방향
		Vec2 vCurDir = GetVelocity();
		float speed = vCurDir.Length();
		vCurDir.Normalize();

		Vec2 vTargetDir = m_TargetObj->GetPos() - GetPos();

		if (0.f < vCurDir.x * vTargetDir.y - vCurDir.y * vTargetDir.x)
		{
			vCurDir = RotateDir(vCurDir, m_RotateSpeed * DT);
		}
		else
		{
			vCurDir = RotateDir(vCurDir, -m_RotateSpeed * DT);
		}

		SetVelocity(vCurDir, speed);
	}


}

void CKiMissile::BeginOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider)
{

	if (L"Monster" == _OtherObj->GetName())
	{
		CAttack* Attack = new CAttack;
		Attack->SetPos(GetPos());
		Attack->SetScale(1.f, 1.f);
		Attack->SetName(L"Attack");
		CreateObject(Attack, LAYER_TYPE::PLAYER_PROJECTILE);

	

		DeleteObject(this);
	}
}