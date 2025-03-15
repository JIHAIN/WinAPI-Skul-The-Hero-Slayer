#include "pch.h"
#include "Snap_Enter_State.h"

#include "Boss_Monster.h"
#include "CAnimator.h"
#include "BossLeft.h"
#include "BossRight.h"
#include "Boss_Body.h"
#include "Boss_Chin.h"
#include "CTimeMgr.h"

#include "CSound.h"
#include "CAssetMgr.h"

Snap_Enter_State::Snap_Enter_State()
	: first(true)
	, AccTime(0.f)
{
}

Snap_Enter_State::~Snap_Enter_State()
{
}


void Snap_Enter_State::Enter()
{
	Boss_Monster* BossMonster = GetOwner<Boss_Monster>();

	Boss_Part Part;

	BossMonster->GetPart(Part);
	CAnimator* BodyAni = Part.Body->GetMonsterAnimator();
	CAnimator* LeftAni = Part.Hand_Left->GetMonsterAnimator();
	CAnimator* RightAni = Part.Hand_Right->GetMonsterAnimator();

	BodyAni->Play(L"Boss_Body1", true);
	LeftAni->Play(L"Boss_Hand1_Left_Snap", false);
	RightAni->Play(L"Boss_Hand1_Right_Snap", false);


	Vec2 Boss_Pos = BossMonster->GetPos();
	Vec2 Left_handPos = Part.Hand_Left->GetPos();
	Vec2 Right_handPos = Part.Hand_Right->GetPos();

	float groundLevel = 1550.f;  // 땅의 Y 좌표
	float HandGoal = 1000.f; // 손 세팅위치

	//처음 시도 된거라면 
	
	CSound* m_Sound = CAssetMgr::Get()->LoadSound(L"ElderEnt_Sweeping_Ready", L"sound\\ElderEnt_Sweeping_Ready.wav");
	m_Sound->Play();

	if (first)
	{
		Part.Hand_Left->SetPos(Boss_Pos.x - (HandGoal), groundLevel);
	}
	else
	{
		Part.Hand_Right->SetPos(Boss_Pos.x + HandGoal, groundLevel);
	}

}

void Snap_Enter_State::FinalTick()
{
	Boss_Monster* BossMonster = GetOwner<Boss_Monster>();

	Boss_Part Part;

	BossMonster->GetPart(Part);

	CAnimator* ChinAni = Part.Chin->GetMonsterAnimator();
	CAnimator* HeadAni = BossMonster->GetMonsterAnimator();
	
	

	AccTime += DT;

	if (AccTime >= 1.f)
	{
		CSound* m_Sound = CAssetMgr::Get()->LoadSound(L"ElderEnt_Sweeping_Roar", L"sound\\ElderEnt_Sweeping_Roar.wav");
		m_Sound->Play();

		AccTime = 0.f; // 시간 초기화
		ChinAni->Play(L"Boss_Chin1_Row", false);
		HeadAni->Play(L"Boss_Head1_Atk", false);
		GetStateMachine()->ChangeState(L"Snaping");
	}



	
}

void Snap_Enter_State::Exit()
{
	if (first == true)
	{
		first = false;
	}
	else
	{
		first = true;
	}
}