#include "pch.h"
#include "Boss_Ball_Create.h"
#include "CAnimator.h"
#include "CTimeMgr.h"

#include "Boss_Ball_Obj.h"
#include "CLevel.h"
#include "CLevelMgr.h"

Boss_Ball_Create::Boss_Ball_Create(Vec2 _Pos)
	: m_Animator(nullptr)
	, LifeTime(0.f)
{
	m_Animator = AddComponent(new CAnimator);
	
	m_Animator->LoadAnimation(L"animation\\Effect\\Boss_Ball_Create.anim");
	
	m_Animator->Play(L"Boss_Ball_Create", false);

	CLevel* CurLevel = CLevelMgr::Get()->GetCurrentLevel();

	Boss_Ball_Obj* Boss_Obj = new Boss_Ball_Obj;
	Boss_Obj->SetPos(_Pos.x, _Pos.y + 10.f);
	CurLevel->AddObject(Boss_Obj, LAYER_TYPE::MONSTER_PROJECTILE);

}

Boss_Ball_Create::Boss_Ball_Create(const Boss_Ball_Create& _Other)
	: CObj(_Other)
	, m_Animator(nullptr)
	, LifeTime (0.f)
{
	m_Animator = GetComponent<CAnimator>();
}

Boss_Ball_Create::~Boss_Ball_Create()
{
}

void Boss_Ball_Create::Tick()
{
	LifeTime += DT;

	if (LifeTime > 1.7f)
	{
		DeleteObject(this);
	}
}

void Boss_Ball_Create::Render()
{
	if (m_Animator != nullptr)
	{
		m_Animator->Render();
	}
}