#include "pch.h"
#include "Stamp_End_State.h"

#include "Boss_Monster.h"
#include "CAnimator.h"
#include "BossLeft.h"
#include "BossRight.h"
#include "Boss_Body.h"
#include "Boss_Chin.h"

#include "CAssetMgr.h"
#include "CSound.h"


Stamp_End_State::Stamp_End_State()
	:	AccTime(0.f)
{
}

Stamp_End_State::~Stamp_End_State()
{
}


void Stamp_End_State::Enter()
{
	Boss_Monster* BossMonster = GetOwner<Boss_Monster>();
	Boss_Part Part;

	BossMonster->GetPart(Part);
	CAnimator* BodyAni = Part.Body->GetMonsterAnimator();
	CAnimator* ChinAni = Part.Chin->GetMonsterAnimator();
	CAnimator* LeftAni = Part.Hand_Left->GetMonsterAnimator();
	CAnimator* RightAni = Part.Hand_Right->GetMonsterAnimator();

	BodyAni->Play(L"Boss_Body1", true);
	LeftAni->Play(L"Boss_Hand1_Left_Stamping", true);
	RightAni->Play(L"Boss_Hand1_Right_Stamping", true);

	
}

void Stamp_End_State::FinalTick()
{
	AccTime += DT;

	if (AccTime >= 1.2f)
	{
		AccTime = 0.f; // 시간 초기화
		
		GetStateMachine()->ChangeState(L"IdleState");
		
	}
}

void Stamp_End_State::Exit()
{

}