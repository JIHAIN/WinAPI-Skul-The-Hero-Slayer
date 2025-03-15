#include "pch.h"
#include "Dark_Waves.h"
#include "CAnimator.h"
#include "CCollider.h"
#include "CTimeMgr.h"

Dark_Waves::Dark_Waves(bool _Right)
	: m_Acctime(0.f)
	, m_Duration(2.f)
	, m_Collider(nullptr)
	, m_Animator(nullptr)
	, m_Dir(0.f)
{
	// 컴포넌트 추가
	m_Collider = AddComponent(new CCollider);
	m_Animator = AddComponent(new CAnimator);

	m_Animator->LoadAnimation(L"animation\\Effect\\Dark_Wave_Right.anim");
	m_Animator->LoadAnimation(L"animation\\Effect\\Dark_Wave_Left.anim");

	if (_Right)
	{
		m_Animator->Play(L"Dark_Wave_Right", true);
		m_Dir = 1;
	}
	else
	{
		m_Animator->Play(L"Dark_Wave_Left", true);
		m_Dir = -1;
	}
}

Dark_Waves::Dark_Waves(const Dark_Waves& _Other)
	: CObj(_Other)
	, m_Acctime(0.f)
	, m_Animator(nullptr)
	, m_Collider(nullptr)
	, m_Duration(0.f)
{
	m_Collider = (CCollider*)GetComponentByName(L"Player_Attack");
	m_Animator = GetComponent<CAnimator>();
}

Dark_Waves::~Dark_Waves()
{
}



void Dark_Waves::Tick()
{
	m_Acctime += DT;
	float Speed = 500.f;

	float Velo = Speed * m_Dir * DT;

	Vec2 cPos = GetPos();

	cPos.x += Velo;

	if (m_Acctime > m_Duration)
	{
		DeleteObject(this);
	}

	SetPos(cPos);
}

void Dark_Waves::Render()
{
	m_Animator->Render();
}

void Dark_Waves::BeginOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider)
{
}

void Dark_Waves::SetCollider(float _X, float _Y)
{
	m_Collider->SetScale(Vec2(_X, _Y));
}