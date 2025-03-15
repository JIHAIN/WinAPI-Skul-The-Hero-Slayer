#include "pch.h"
#include "Boss_Idle.h"
#include "Boss_Monster.h"

#include "CAnimator.h"
#include "BossLeft.h"
#include "BossRight.h"
#include "Boss_Body.h"
#include "Boss_Chin.h"

#include "CKeyMgr.h"

Boss_Idle::Boss_Idle()
{
}

Boss_Idle::~Boss_Idle()
{
}


void Boss_Idle::Enter()
{
	Boss_Monster* BossMonster = GetOwner<Boss_Monster>();

	Boss_Part Part;

	BossMonster->GetPart(Part);
	CAnimator* BodyAni = Part.Body->GetMonsterAnimator();
	CAnimator* ChinAni = Part.Chin->GetMonsterAnimator();
	CAnimator* LeftAni = Part.Hand_Left->GetMonsterAnimator();
	CAnimator* RightAni = Part.Hand_Right->GetMonsterAnimator();
	CAnimator* HeadAni = BossMonster->GetMonsterAnimator();

	HeadAni->Play(L"Boss_Head1_Idle", false);
	BodyAni->Play(L"Boss_Body1", true);
	ChinAni->Play(L"Boss_Chin1", true);
	LeftAni->Play(L"Boss_Hand1_Left_Idle", true);
	RightAni->Play(L"Boss_Hand1_Right_Idle", true);

	/*HeadAni->Play(L"Boss_Head2_Idle", false);
	BodyAni->Play(L"Boss_Body2_idle", true);
	ChinAni->Play(L"Boss_Chin2", true);
	LeftAni->Play(L"Boss_Hand2_Left_Idle", true);
	RightAni->Play(L"Boss_Hand2_Right_Idle", true);*/



	Part.Hand_Left->SetPos(Part.Hand_Left->GetInitLeftPos());
	Part.Hand_Right->SetPos(Part.Hand_Right->GetInitRightPos());

}

void Boss_Idle::FinalTick()
{
	static bool UpEnd = false; // 머리 흔들기 위쪽 끝에 닿았는지 체크
	static float Time = 0.f;

	Boss_Monster* BossMonster = GetOwner<Boss_Monster>(); // 보스 주소 받아오기

	Boss_Part Part; // 보스 파츠들 구조체

	BossMonster->GetPart(Part);

	Vec2 BossInitPos = BossMonster->GetMonsterInfo().InitPos; // 주축이 될 보스의 이니셜포즈와 각 파츠들 위치 받아오기
	Vec2 bPos = BossMonster->GetPos();
	Vec2 LPos = Part.Hand_Left->GetPos();
	Vec2 RPos = Part.Hand_Right->GetPos();
	Vec2 CPos = Part.Chin->GetPos();

	float moveSpeed = 10 * DT;

	Vec2 Up = Vec2(0.f, -1.f) * moveSpeed;
	Vec2 Down = Vec2(0.f, 1.f) * moveSpeed;

	if (!UpEnd)
	{
		BossMonster->SetPos(bPos + Up);
		Part.Hand_Left->SetPos(LPos + Up);
		Part.Hand_Right->SetPos(RPos + Up);
		Part.Chin->SetPos(CPos + Up);
		
		if (bPos.y < BossInitPos.y - 5.f)
		{
			UpEnd = true;
		}
	}
	if(UpEnd)
	{
		BossMonster->SetPos(bPos + Down);
		Part.Hand_Left->SetPos(LPos + Down);
		Part.Hand_Right->SetPos(RPos + Down);
		Part.Chin->SetPos(CPos + Down);
	
		if (bPos.y > BossInitPos.y + 5.f)
		{
			UpEnd = false;
		}
	}

	Time += DT;

	if (Time > 6.f)
	{
		if (KEY_TAP(KEY::Q))
		{
			GetStateMachine()->ChangeState(L"Stamp_Enter");
		}
		else if (KEY_TAP(KEY::W))
		{
			GetStateMachine()->ChangeState(L"Snap_Enter");
		}
		else if (KEY_TAP(KEY::E))
		{
			GetStateMachine()->ChangeState(L"Ball_Enter_State");
		}
		
		
		//Time = 0.f;
	}
	
}

void Boss_Idle::Exit()
{

}