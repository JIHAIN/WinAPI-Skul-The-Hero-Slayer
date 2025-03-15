#include "pch.h"
#include "Monster_Wall.h"

#include "CMonster.h"
#include "CCollider.h"
#include "CRigidBody.h"

Monster_Wall::Monster_Wall()
	: m_Collider(nullptr)
{
	m_Collider = AddComponent(new CCollider);
}

Monster_Wall::Monster_Wall(const Monster_Wall& _Other)
	: CObj(_Other)
	, m_Collider(nullptr)
{
	m_Collider = GetComponent<CCollider>();
	m_Collider->SetName(L"Monster_Wall");
}

Monster_Wall::~Monster_Wall()
{
}

void Monster_Wall::SetScale(Vec2 _Scale)
{
	CObj::SetScale(_Scale);

	if (m_Collider)
	{
		m_Collider->SetScale(_Scale);
	}
}

void Monster_Wall::SetScale(float _Width, float _Height)
{
	float tileSize = 64;

	CObj::SetScale(tileSize * _Width, tileSize * _Height);

	if (m_Collider)
	{
		m_Collider->SetScale(Vec2(tileSize * _Width, tileSize * _Height - 35.f));
	}
}

void Monster_Wall::SetScaleMouse(Vec2 _Scale)
{
	CObj::SetScale(_Scale);

	if (m_Collider)
	{
		m_Collider->SetScale(_Scale);
	}
}

void Monster_Wall::Tick()
{

}

void Monster_Wall::BeginOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider)
{
	
	if (_OtherObj->GetName() == L"Monster")
	{
		CMonster* pOtherObj2 = dynamic_cast<CMonster*>(_OtherObj);

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

		pOtherObj2->SetWall(true);

	}
}

// 기존 오버랩
void Monster_Wall::Overlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider)
{
	if (_OtherObj->GetName() == L"Monster")
	{
		CMonster* pOtherObj2 = dynamic_cast<CMonster*>(_OtherObj);

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


		// 충돌 깊이가 더 작은 축에 대해 먼저 처리
		if (overlapX < overlapY)
		{
			if (myMax.x < PlaMax.x && myMax.x > PlaMin.x)  //플레이어가 나보다 오른쪽에서 다가오면 그라운드도 꺼줌
			{
				_OtherObj->SetPos(CurPos.x += (ScaleX - vDiff.x), CurPos.y);
			}
			else
			{
				// 플레이어가 나보다 왼쪽에서 오면 그라운드 꺼줌
				_OtherObj->SetPos(CurPos.x -= (ScaleX - vDiff.x), CurPos.y);
			}
		}
	}
}


void Monster_Wall::EndOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider)
{
	CMonster* pOtherObj2 = dynamic_cast<CMonster*>(_OtherObj);
}
