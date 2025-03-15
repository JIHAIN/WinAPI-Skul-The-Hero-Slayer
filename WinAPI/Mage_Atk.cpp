#include "pch.h"
#include "Mage_Atk.h"

#include "CAnimator.h"
#include "CAnim.h"
#include "CTimeMgr.h"
#include "CPathMgr.h"
#include "CCollider.h"
#include "CPlayer.h"

Mage_Atk::Mage_Atk()
	: m_PlayerPos(0,0)
	, AccTime(0.f)
	, m_Dir(0,0)
	, Deleteing(false)
{
	m_Collider = AddComponent(new CCollider);
	m_Animator = AddComponent(new CAnimator);

	m_Duration = 2.f;

	m_Collider->SetScale(Vec2(40.f, 40.f));

	m_Animator->LoadAnimation(L"animation\\Monster\\Mage_ATK_Fireball_Careat.anim");
	m_Animator->LoadAnimation(L"animation\\Monster\\Mage_ATK_Fireball_Left.anim");
	m_Animator->LoadAnimation(L"animation\\Monster\\Mage_ATK_Fireball_Right.anim");
	m_Animator->LoadAnimation(L"animation\\Monster\\Mage_ATK_Fireball_Left_Delete.anim");
	m_Animator->LoadAnimation(L"animation\\Monster\\Mage_ATK_Fireball_Right_Delete.anim");


	m_Animator->Play(L"Mage_ATK_Fireball_Careat", false);


}

Mage_Atk::Mage_Atk(const Mage_Atk& _Other)
{
}

Mage_Atk::~Mage_Atk()
{
	
}

void Mage_Atk::Tick()
{
	AccTime += DT;

	Vec2 myPos = GetPos();
	//m_Dir = m_PlayerPos - myPos;
	//m_Dir.Normalize();


	if (AccTime >= 1.f)
	{
		Vec2 m_Velocity = m_Dir;
		m_Velocity *= 600.f;

		myPos += m_Velocity * DT;
		SetPos(myPos);

		if (m_Dir.x < 0.f && Deleteing == false) // 상대가 왼쪽 이라면
		{
			m_Animator->Play(L"Mage_ATK_Fireball_Left", true);
		}
		else if (m_Dir.x > 0.f && Deleteing == false)
		{
			m_Animator->Play(L"Mage_ATK_Fireball_Right", true);
		}
	}

	if (AccTime >= 2.f)
	{
		if (m_Dir.x < 0.f) // 상대가 왼쪽 이라면
		{
			Deleteing = true;
			m_Animator->Play(L"Mage_ATK_Fireball_Left_Delete", false);
		}
		else
		{ 
			Deleteing = true;
			m_Animator->Play(L"Mage_ATK_Fireball_Right_Delete", false);
		}

		if (AccTime >= 3.5f)
		{
			DeleteObject(this);
		}

	}
}

void Mage_Atk::BeginOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider)
{
	if (_OtherCollider->GetName() == L"Player")
	{

	}
}

void Mage_Atk::TargetDir(Vec2 _my, Vec2 _Taget)
{
	Vec2 vDir = _Taget - _my;
	vDir.Normalize();
	m_Dir = vDir;
}
