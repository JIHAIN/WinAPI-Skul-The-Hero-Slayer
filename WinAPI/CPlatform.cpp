#include "pch.h"
#include "CPlatform.h"

#include "CPlayer.h"
#include "CMonster.h"
#include "CCollider.h"
#include "CRigidBody.h"

#include "CSound.h"
#include "CAssetMgr.h"

CPlatform::CPlatform()
	: m_Collider(nullptr)
{
	m_Collider = AddComponent(new CCollider);
}

CPlatform::CPlatform(const CPlatform& _Other)
	: CObj(_Other)
	, m_Collider(nullptr)
{
	m_Collider = GetComponent<CCollider>();
	m_Collider->SetName(L"Platform");
}

CPlatform::~CPlatform()
{
}

void CPlatform::SetScale(Vec2 _Scale)
{
	CObj::SetScale(_Scale);

	if (m_Collider)
	{
		m_Collider->SetScale(_Scale);
	}
}

void CPlatform::SetScale(float _Width, float _Height)
{
	float tileSize = 64;

	CObj::SetScale(tileSize * _Width, tileSize * _Height);

	if (m_Collider)
	{
		m_Collider->SetScale(Vec2(tileSize * _Width, tileSize * _Height -35.f));
	}
}

void CPlatform::SetScaleMouse(Vec2 _Scale)
{
	CObj::SetScale(_Scale);

	if (m_Collider)
	{
		m_Collider->SetScale(_Scale);
	}
}

void CPlatform::Tick()
{

}

void CPlatform::BeginOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider)
{
	CPlayer* pOtherObj = dynamic_cast<CPlayer*>(_OtherObj);
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
	// 상대 콜라이더의 min 왼쪽, 아래 : max 오른쪽, 위
	Vec2 PlaMin = Vec2(PlaPos.x - PlaScale.x / 2.f, PlaPos.y + PlaScale.y / 2.f);
	Vec2 PlaMax = Vec2(PlaPos.x + PlaScale.x / 2.f, PlaPos.y - PlaScale.y / 2.f);
	// 내 콜라이더의 min 왼쪽, 아래 : max 오른쪽, 위
	Vec2 myMin = Vec2(myPos.x - mySclae.x / 2.f, myPos.y + mySclae.y / 2.f);
	Vec2 myMax = Vec2(myPos.x + mySclae.x / 2.f, myPos.y - mySclae.y / 2.f);

	if (pOtherObj)
	{
		if (myMax.y > PlaMin.y&& PlaMax.y > myMax.y)
		{
			// 플레이어가 나보다 아래에서 오면 그라운드 꺼줌
			CRigidBody* pRigidBody = pOtherObj->GetComponent<CRigidBody>();
			pRigidBody->SetGround(false);
		}
		else
		{
			CRigidBody* pRigidBody = pOtherObj->GetComponent<CRigidBody>();
			pRigidBody->SetGround(true);
		}
		
		CSound* m_Sound = CAssetMgr::Get()->LoadSound(L"Landing", L"sound\\Default_Landing.wav");
		m_Sound->Play();

	}

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

Vec2 CPlatform::CalculateCollisionNormal(CCollider* _OwnCollider, CCollider* _OtherCollider)
{
	Vec2 platformPos = _OwnCollider->GetFinalPos();
	Vec2 platformScale = _OwnCollider->GetScale();
	Vec2 playerPos = _OtherCollider->GetFinalPos();
	Vec2 playerScale = _OtherCollider->GetScale();

	Vec2 distance = playerPos - platformPos;

	// 충돌 깊이 계산
	float overlapX = (platformScale.x / 2 + playerScale.x / 2) - abs(distance.x);
	float overlapY = (platformScale.y / 2 + playerScale.y / 2) - abs(distance.y);

	// 더 작은 오버랩을 가진 축이 충돌 방향
	if (overlapX < overlapY)
	{
		return Vec2(distance.x > 0 ? 1 : -1, 0);
	}
	else
	{
		return Vec2(0, distance.y > 0 ? 1 : -1);
	}
}

// 기존 오버랩
void CPlatform::Overlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider)
{
	if (_OtherObj->GetName() == L"Player")
	{

		CPlayer* pOtherObj = dynamic_cast<CPlayer*>(_OtherObj);
		
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

		// 상대 콜라이더의 min 왼쪽, 아래 : max 오른쪽, 위
		Vec2 PlaMin = Vec2(PlaPos.x - PlaScale.x / 2.f, PlaPos.y + PlaScale.y / 2.f);
		Vec2 PlaMax = Vec2(PlaPos.x + PlaScale.x / 2.f, PlaPos.y - PlaScale.y / 2.f);

		// 내 콜라이더의 min 왼쪽, 아래 : max 오른쪽, 위
		Vec2 myMin = Vec2(myPos.x - mySclae.x / 2.f, myPos.y + mySclae.y / 2.f);
		Vec2 myMax = Vec2(myPos.x + mySclae.x / 2.f, myPos.y - mySclae.y / 2.f);

		CRigidBody* pRigidBody = pOtherObj->GetComponent<CRigidBody>();

		// 이전 프레임과 현재 프레임 모두 땅에 있었다면 위치 보정 없이 리턴
		if (pRigidBody->IsGround() && pRigidBody->WasGround())
		{
			return;
		}

		// 충돌 깊이가 더 작은 축에 대해 먼저 처리
		if (overlapX < overlapY)
		{
			if (myMax.x < PlaMax.x && myMax.x > PlaMin.x)  //플레이어가 나보다 오른쪽에서 다가오면 그라운드도 꺼줌
			{
				_OtherObj->SetPos(CurPos.x += (ScaleX - vDiff.x), CurPos.y);

				if (pOtherObj)
				{
					CRigidBody* pRigidBody = pOtherObj->GetComponent<CRigidBody>();
					pRigidBody->SetGround(false);
				
				}
			}
			else
			{
				// 플레이어가 나보다 왼쪽에서 오면 그라운드 꺼줌
				_OtherObj->SetPos(CurPos.x -= (ScaleX - vDiff.x), CurPos.y);
				if (pOtherObj)
				{
					CRigidBody* pRigidBody = pOtherObj->GetComponent<CRigidBody>();
					pRigidBody->SetGround(false);
				}
			}
		}
		else
		{
			if (myMax.y < PlaMin.y && PlaMax.y < myMax.y) //플레이어가 나보다 위 
			{
				_OtherObj->SetPos(CurPos.x, CurPos.y -= (ScaleY - vDiff.y) - 1.f);

				if (pOtherObj)
				{
					CRigidBody* pRigidBody = pOtherObj->GetComponent<CRigidBody>();
					pRigidBody->SetGround(true);
				}
			}
			else
			{
				// 플레이어가 나보다 아래에서 오면 그라운드 꺼줌
				_OtherObj->SetPos(CurPos.x, CurPos.y += (ScaleY - vDiff.y));
				if (pOtherObj)
				{
					CRigidBody* pRigidBody = pOtherObj->GetComponent<CRigidBody>();
					pRigidBody->SetGround(false);
				}
			}
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


void CPlatform::EndOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider)
{
	CPlayer* pOtherObj = dynamic_cast<CPlayer*>(_OtherObj);
	CMonster* pOtherObj2 = dynamic_cast<CMonster*>(_OtherObj);

	if (pOtherObj)
	{
		CRigidBody* pRigidBody = pOtherObj->GetComponent<CRigidBody>();
		pRigidBody->SetGround(false);
	}
	if (pOtherObj2)
	{
		CRigidBody* pRigidBody2 = pOtherObj2->GetComponent<CRigidBody>();
		pRigidBody2->SetGround(false);
	}

	
}

