#include "pch.h"
#include "Boss_Door.h"
#include "CAnimator.h"
#include "CCollider.h"

#include "CLevel.h"
#include "CLevelMgr.h"
#include "CLevel_Stage01.h"
#include "CLevel_Stage02.h"
#include "CLevel_Stage03.h"
#include "CLevel_Stage04.h"
#include "CCamera.h"

#include "CKeyMgr.h"

Boss_Door::Boss_Door()
	: m_Animator(nullptr)
	, m_Collider(nullptr)
	, Door_On(true)
{
	m_Animator = AddComponent(new CAnimator);

	m_Animator->LoadAnimation(L"animation\\Effect\\Boss_Door.anim");

	m_Animator->Play(L"Boss_Door", true);

	m_Collider = AddComponent(new CCollider);
	m_Collider->SetName(L"Door");
	m_Collider->SetScale(Vec2(150.f, 200.f));
	m_Collider->SetOffset(Vec2(0.f, 30.f));
}

Boss_Door::Boss_Door(const Boss_Door& _Other)
{
}

Boss_Door::~Boss_Door()
{
}

void Boss_Door::Render()
{
	m_Animator->Render();
}

void Boss_Door::BeginOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider)
{
}

void Boss_Door::Overlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider)
{
	if (Door_On)
	{
		if (_OtherObj->GetName() == L"Player")
		{
			if (KEY_TAP(KEY::X))
			{
				CCamera::Get()->SetLookAt(Vec2(191.f, 1500.f));
				ChangeLevel(LEVEL_TYPE::STAGE_05);
			}
		}
	}

}