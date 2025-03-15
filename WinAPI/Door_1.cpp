#include "pch.h"
#include "Door_1.h"
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

Door_1::Door_1()
	: m_Animator(nullptr)
	, m_Collider(nullptr)
	, Door_On(true)
{
	m_Animator = AddComponent(new CAnimator);

	m_Animator->LoadAnimation(L"animation\\Effect\\Door_Not.anim");
	m_Animator->LoadAnimation(L"animation\\Effect\\Door_Open.anim");

	m_Animator->Play(L"Door_Open", true);

	m_Collider = AddComponent(new CCollider);
	m_Collider->SetName(L"Door");
	m_Collider->SetScale(Vec2(100.f, 200.f));
	m_Collider->SetOffset(Vec2(0.f, 20.f));
}

Door_1::Door_1(const Door_1& _Other)
{
}

Door_1::~Door_1()
{
}

void Door_1::Render()
{
	m_Animator->Render();
}

void Door_1::BeginOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider)
{
}

void Door_1::Overlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider)
{
	if (Door_On)
	{
		if (_OtherObj->GetName() == L"Player")
		{
			if (KEY_TAP(KEY::X))
			{
				CLevel* CurLevel = CLevelMgr::Get()->GetCurrentLevel();

				CLevel_Stage01* Stage1 = dynamic_cast<CLevel_Stage01*>(CurLevel);

				if (Stage1)
				{
					CCamera::Get()->SetLookAt(Vec2(191.f,1500.f));
					ChangeLevel(LEVEL_TYPE::STAGE_02);
				}

				CLevel_Stage02* Stage2 = dynamic_cast<CLevel_Stage02*>(CurLevel);
				
				if(Stage2)
				{
					CCamera::Get()->SetLookAt(Vec2(155.f, 1057.f));
					ChangeLevel(LEVEL_TYPE::STAGE_03);
				}

				CLevel_Stage03* Stage3 = dynamic_cast<CLevel_Stage03*>(CurLevel);

				if(Stage3)
				{
					CCamera::Get()->SetLookAt(Vec2(274.f, 1580.f));
					ChangeLevel(LEVEL_TYPE::STAGE_04);
				}

			}
		}
	}
	
}