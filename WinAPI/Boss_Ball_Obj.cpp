#include "pch.h"
#include "Boss_Ball_Obj.h"

#include "CEngine.h"
#include "CAssetMgr.h"

#include "CCollider.h"
#include "CAnimator.h"
#include "CAnim.h"
#include "CTimeMgr.h"
#include "CPathMgr.h"

#include "CSound.h"
#include "CAssetMgr.h"



Boss_Ball_Obj::Boss_Ball_Obj()
	: m_AccTime(0.f)
	, m_DownTime(0.f)
	, m_Collider(nullptr)
	, m_Animator(nullptr)
	, Sound1(true)
{
	// 컴포넌트 추가
	m_Collider = AddComponent(new CCollider);
	m_Animator = AddComponent(new CAnimator);

	m_Collider->SetScale(Vec2 (100.f, 100.f));
	
	m_Animator->LoadAnimation(L"animation\\Effect\\Boss_Ball_Atk.anim");
	m_Animator->LoadAnimation(L"animation\\Effect\\Boss_Ball_End.anim");

	m_Animator->Play(L"Boss_Ball_Atk", false);
}

Boss_Ball_Obj::Boss_Ball_Obj(const Boss_Ball_Obj& _Other)
	: CObj(_Other)
	, m_AccTime(0.f)
	, m_Animator(nullptr)
	, m_Collider(nullptr)
	, m_DownTime(0.f)
{
	m_Collider = (CCollider*)GetComponentByName(L"Attack");
	m_Animator = GetComponent<CAnimator>();
}

Boss_Ball_Obj::~Boss_Ball_Obj()
{

}

void Boss_Ball_Obj::BeginOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider)
{

}

void Boss_Ball_Obj::Tick()
{
	Vec2 bPos = GetPos();

	m_DownTime += DT;

	int a = 0;

	float DownPos = 1660.f;

	if (m_DownTime > 1.8 && bPos.y < DownPos)
	{
		bPos.y += 1000.f * DT;
	}

	else if (bPos.y > DownPos)
	{
		m_Animator->Play(L"Boss_Ball_End", false);

		m_AccTime += DT;

		if (Sound1)
		{
			Sound1 = false;
			CSound* m_Sound = CAssetMgr::Get()->LoadSound(L"Atk_Explosion_Large", L"sound\\Atk_Explosion_Large.wav");
			m_Sound->Play();
			
		}
	
		if (m_AccTime > 1.3f)
		{
			DeleteObject(this);
		}
	}
	SetPos(bPos);
}

void Boss_Ball_Obj::Render()
{
	m_Animator->Render();
}



void Boss_Ball_Obj::SetCollider(float _X, float _Y)
{
	m_Collider->SetScale(Vec2(_X, _Y));
}