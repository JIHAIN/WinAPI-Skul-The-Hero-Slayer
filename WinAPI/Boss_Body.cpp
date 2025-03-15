#include "pch.h"
#include "Boss_Body.h"

#include "CEngine.h"
#include "CCollider.h"
#include "CRigidBody.h"
#include "CTimeMgr.h"
#include "CAnimator.h"
#include "CAssetMgr.h"
#include "CPlayer.h"

Boss_Body::Boss_Body()
	: Left_Collider(nullptr)
	, Right_Collider(nullptr)
	, m_Animator(nullptr)
	, m_Info{}
	, m_TargetObj(nullptr)
	, flor_On(true)
{
	m_Animator = AddComponent(new CAnimator);

	m_Animator->LoadAnimation(L"animation\\Boss\\Boss_Body1.anim");
	m_Animator->LoadAnimation(L"animation\\Boss\\Boss_Body2_idle.anim");

	m_Animator->Play(L"Boss_Body1", true);

	Left_Collider = AddComponent(new CCollider);
	Left_Collider->SetName(L"Left_flor");
	Left_Collider->SetScale(Vec2(180.f, 10.f));
	Left_Collider->SetOffset(Vec2(-130.f, 120.f));

	Right_Collider = AddComponent(new CCollider);
	Right_Collider->SetName(L"Right_flor");
	Right_Collider->SetScale(Vec2(180.f, 10.f));
	Right_Collider->SetOffset(Vec2(200.f, 60.f));


}			

Boss_Body::Boss_Body(const Boss_Body& _Other)
{

}

Boss_Body::~Boss_Body()
{
}


void Boss_Body::Tick()
{
}


void Boss_Body::Render_Part()
{
	m_Animator->Render();
}

void Boss_Body::BeginOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider)
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

	if (pOtherObj && flor_On)
	{
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

}

void Boss_Body::Overlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider)
{
}

void Boss_Body::EndOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider)
{
	CPlayer* pOtherObj = dynamic_cast<CPlayer*>(_OtherObj);
	if (pOtherObj && flor_On)
	{
		CRigidBody* pRigidBody = pOtherObj->GetComponent<CRigidBody>();
		pRigidBody->SetGround(false);
	}
}
