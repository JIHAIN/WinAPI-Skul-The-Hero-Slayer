#include "pch.h"
#include "Cfloor.h"

#include "CEngine.h"
#include "CPlayer.h"
#include "CMonster.h"
#include "CCollider.h"
#include "CRigidBody.h"

#include "CSound.h"
#include "CAssetMgr.h"

Cfloor::Cfloor()
	: m_Collider(nullptr)
{
	m_Collider = AddComponent(new CCollider);
}

Cfloor::Cfloor(const Cfloor& _Other)
	: CObj(_Other)
	, m_Collider(nullptr)
{
	m_Collider = GetComponent<CCollider>();
	m_Collider->SetName(L"Floor");
}

Cfloor::~Cfloor()
{
}

void Cfloor::SetScale(Vec2 _Scale)
{
	CObj::SetScale(_Scale);

	if (m_Collider)
	{
		m_Collider->SetScale(_Scale);
	}
}

void Cfloor::SetScale(float _Width, float _Height)
{
	float tileSize = 64;
	
	CObj::SetScale(tileSize * _Width, tileSize * _Height /2);
	
	if (m_Collider)
	{
		// m_Collider->SetScale(Vec2(tileSize * _Width, tileSize * _Height/2 - 12.f));
		m_Collider->SetScale(Vec2(tileSize * _Width, tileSize * _Height / 2 ));
	}
}

void Cfloor::Tick()
{
}


void Cfloor::BeginOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider)
{
	Vec2 myPos = _OwnCollider->GetFinalPos();
	Vec2 mySclae = _OwnCollider->GetScale();
	Vec2 PlaPos = _OtherCollider->GetFinalPos();
	Vec2 PlaScale = _OtherCollider->GetScale();

	// 상대 콜라이더의 min 왼쪽, 위 : max 오른쪽, 아래
	Vec2 PlaMin = Vec2(PlaPos.x - PlaScale.x / 2.f, PlaPos.y - PlaScale.y / 2.f);
	Vec2 PlaMax = Vec2(PlaPos.x + PlaScale.x / 2.f, PlaPos.y + PlaScale.y / 2.f);

	// 내 콜라이더의 min 왼쪽, 위 : max 오른쪽, 아래
	Vec2 myMin = Vec2(myPos.x - mySclae.x / 2.f, myPos.y - mySclae.y / 2.f);
	Vec2 myMax = Vec2(myPos.x + mySclae.x / 2.f, myPos.y + mySclae.y / 2.f);

	CPlayer* pOtherObj = dynamic_cast<CPlayer*>(_OtherObj);
	if (pOtherObj)
	{
		CSound* m_Sound = CAssetMgr::Get()->LoadSound(L"Landing", L"sound\\Default_Landing.wav");
		m_Sound->Play();

		if (myMin.y <= PlaMax.y && myMax.y >= PlaMax.y)
		{
		 CRigidBody* pRigidBody = pOtherObj->GetComponent<CRigidBody>();
		 pRigidBody->SetGround(true);
		}
		else
		{
			CRigidBody* pRigidBody = pOtherObj->GetComponent<CRigidBody>();
			pRigidBody->SetGround(false);
		}
	}

	if (_OtherObj->GetName() == L"Monster")
	{
		CMonster* pOtherObj2 = dynamic_cast<CMonster*>(_OtherObj);
		Vec2 vDiff = _OwnCollider->GetFinalPos() - _OtherCollider->GetFinalPos();

		CRigidBody* pRigidBody2 = pOtherObj2->GetComponent<CRigidBody>();
		pRigidBody2->SetGround(true);

		// 두 충돌체의 각 축별 거리값
		vDiff.x = abs(vDiff.x);
		vDiff.y = abs(vDiff.y);

		// 두 충돌체의 크기를 절반씩 합친 값
		float ScaleX = _OwnCollider->GetScale().x / 2.f + _OtherCollider->GetScale().x / 2.f;
		float ScaleY = _OwnCollider->GetScale().y / 2.f + _OtherCollider->GetScale().y / 2.f;

		// 충돌 깊이 계산
		float overlapX = ScaleX - vDiff.x;
		float overlapY = ScaleY - vDiff.y;

		Vec2 CurPos = _OtherObj->GetPos();
		Vec2 myPos = _OwnCollider->GetFinalPos();
		Vec2 mySclae = _OwnCollider->GetScale();
		Vec2 PlaPos = _OtherCollider->GetFinalPos();
		Vec2 PlaScale = _OtherCollider->GetScale();

		pOtherObj2->SetPos(CurPos.x, CurPos.y -= (ScaleY - vDiff.y));

	}
}

void Cfloor::Overlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider)
{
	if (_OtherObj->GetName() == L"Monster")
	{
		CMonster* pOtherObj2 = dynamic_cast<CMonster*>(_OtherObj);

		CRigidBody* pRigidBody2 = pOtherObj2->GetComponent<CRigidBody>();
		pRigidBody2->SetGround(true);
		
	
		Vec2 vDiff = _OwnCollider->GetFinalPos() - _OtherCollider->GetFinalPos();

		// 두 충돌체의 각 축별 거리값
		vDiff.x = abs(vDiff.x);
		vDiff.y = abs(vDiff.y);

		// 두 충돌체의 크기를 절반씩 합친 값
		float ScaleX = _OwnCollider->GetScale().x / 2.f + _OtherCollider->GetScale().x / 2.f;
		float ScaleY = _OwnCollider->GetScale().y / 2.f + _OtherCollider->GetScale().y / 2.f;

		// 충돌 깊이 계산
		float overlapX = ScaleX - vDiff.x;
		float overlapY = ScaleY - vDiff.y;

		Vec2 CurPos = _OtherObj->GetPos();
		Vec2 myPos = _OwnCollider->GetFinalPos();
		Vec2 mySclae = _OwnCollider->GetScale();
		Vec2 PlaPos = _OtherCollider->GetFinalPos();
		Vec2 PlaScale = _OtherCollider->GetScale();

		pOtherObj2->SetPos(CurPos.x, CurPos.y -= (ScaleY - vDiff.y));

	}
}

void Cfloor::EndOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider)
{
	CPlayer* pOtherObj = dynamic_cast<CPlayer*>(_OtherObj);
	if (pOtherObj)
	{
		CRigidBody* pRigidBody = pOtherObj->GetComponent<CRigidBody>();
		pRigidBody->SetGround(false);
	}
}


