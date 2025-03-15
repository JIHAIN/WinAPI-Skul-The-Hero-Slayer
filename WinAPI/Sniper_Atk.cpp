#include "pch.h"
#include "Sniper_Atk.h"

#include "CAnimator.h"
#include "CAnim.h"
#include "CTimeMgr.h"
#include "CPathMgr.h"
#include "CCollider.h"
#include "CPlayer.h"

Sniper_Atk::Sniper_Atk(bool _Left)
	: m_PlayerPos(0, 0)
	, AccTime(0.f)
	, m_Dir(0, 0)
	, Deleteing(false)
{
	m_Collider = AddComponent(new CCollider);
	m_Animator = AddComponent(new CAnimator);

	m_Duration = 2.f;

	m_Collider->SetScale(Vec2(40.f, 15.f));

	m_Animator->LoadAnimation(L"animation\\Monster\\Sniper_Atk_Left.anim");
	m_Animator->LoadAnimation(L"animation\\Monster\\Sniper_Atk_Right.anim");
	m_Animator->LoadAnimation(L"animation\\Monster\\Sniper_Look_Left.anim");
	m_Animator->LoadAnimation(L"animation\\Monster\\Sniper_Look_Right.anim");
	
	if (_Left)
	{
		m_Animator->Play(L"Sniper_Look_Left", false);
	}
	else
	{
		m_Animator->Play(L"Sniper_Look_Right", false);
	}


}

Sniper_Atk::Sniper_Atk(const Sniper_Atk& _Other)
{
}

Sniper_Atk::~Sniper_Atk()
{

}

void Sniper_Atk::Tick()
{
	AccTime += DT;

	Vec2 myPos = GetPos();

	Vec2 m_Velocity = m_Dir;
	m_Velocity *= 900.f;

	

	if (AccTime >= 1.f)
	{
		if (m_Dir.x < 0.f) // 상대가 왼쪽 이라면
		{
			m_Animator->Play(L"Sniper_Atk_Left", true);
		}
		else if (m_Dir.x > 0.f)
		{
			m_Animator->Play(L"Sniper_Atk_Right", true);
		}

		myPos += m_Velocity * DT;

		SetPos(myPos);
	}




	if (AccTime >= 3.f)
	{
		DeleteObject(this);
	}
}

void Sniper_Atk::BeginOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider)
{
}

void Sniper_Atk::TargetDir(Vec2 _my, Vec2 _Taget)
{
	// x 값만 사용하게함
	Vec2 vDir = _Taget - _my;
	vDir.Normalize();
	vDir.y = 0.f;

	m_Dir = vDir;
}
