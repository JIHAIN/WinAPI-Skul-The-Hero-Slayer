#include "pch.h"
#include "Sword_Atk_Obj.h"

#include "CAnimator.h"
#include "CAnim.h"
#include "CTimeMgr.h"
#include "CPathMgr.h"
#include "CCollider.h"
#include "CPlayer.h"

Sword_Atk_Obj::Sword_Atk_Obj()
	: m_PlayerPos(0, 0)
	, AccTime(0.f)
	, m_Dir(0, 0)
	, Deleteing(false)
{
	m_Collider = AddComponent(new CCollider);


	m_Duration = 0.4f;

	m_Collider->SetScale(Vec2(50.f, 50.f));

}

Sword_Atk_Obj::Sword_Atk_Obj(const Sword_Atk_Obj& _Other)
{
}

Sword_Atk_Obj::~Sword_Atk_Obj()
{

}

void Sword_Atk_Obj::Tick()
{
	AccTime += DT;

	if (AccTime >= m_Duration)
	{
		DeleteObject(this);
	}
}

void Sword_Atk_Obj::BeginOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider)
{
}

void Sword_Atk_Obj::TargetDir(Vec2 _my, Vec2 _Taget)
{
	// x 값만 사용하게함
	Vec2 vDir = _Taget - _my;
	vDir.Normalize();
	vDir.y = 0.f;

	m_Dir = vDir;
}