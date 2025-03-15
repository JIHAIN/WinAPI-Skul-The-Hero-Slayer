#include "pch.h"
#include "BEnt_Idle_State.h"

#include "CAnimator.h"
#include "CMonster.h"

BEnt_Idle_State::BEnt_Idle_State()
	: MoveOn(false)
	, RightEnd(false)
	, LeftEnd(false)
{
}

BEnt_Idle_State::~BEnt_Idle_State()
{
}

void BEnt_Idle_State::Enter()
{
	//LOG(LOG_LEVEL::WARNING, L"Enter IdleState");
}

void BEnt_Idle_State::FinalTick()
{
	// 몬스터의 인식범위 내에 Player 가 있는지 확인한다.
	CMonster* pMonster = GetOwner<CMonster>();

	float DetectRange = pMonster->GetMonsterInfo().DetectRange;

	// DetecteRange 디버그 렌더링
	DrawDebugShape(DEBUG_SHAPE::CIRCLE, PEN_TYPE::BLUE, BRUSH_TYPE::HOLLOW
		, pMonster->GetRenderPos(), Vec2(DetectRange * 2.f, DetectRange * 2.f));


	// 만약에 인식범위 내에 Player 가 있었으면 자신의 상태를 TraceState 상태로 변경한다.
	CPlayer* pPlayer = CLevelMgr::Get()->GetCurrentLevel()->GetPlayer();

	float Distance = pMonster->GetPos().Distance(pPlayer->GetPos());

	Vec2 MonPos = pMonster->GetPos();
	Vec2 MonScale = pMonster->GetScale();
	Vec2 PlePos = pPlayer->GetPos();
	Vec2 PleScale = pPlayer->GetScale();

	Vec2 InitPos = pMonster->GetMonsterInfo().InitPos;			// 처음위치
	float reconRange = pMonster->GetMonsterInfo().reconRange;	// 정찰거리

	Vec2 RightPos = pMonster->GetPos() + Vec2(1.f, 0.f) * pMonster->GetMonsterInfo().Speed * DT; // 오른쪽으로 움직이게 세팅
	Vec2 LeftPos = pMonster->GetPos() + Vec2(-1.f, 0.f) * pMonster->GetMonsterInfo().Speed * DT; // 왼쪽으로 움직이게 세팅

	if (MonPos.x > InitPos.x - reconRange && !LeftEnd)  // 내 현재 위치가 왼쪽 정찰거리 만큼 못갔는지 체크.
	{
		CAnimator* MonAni = pMonster->GetMonsterAnimator();
		MonAni->Play(L"BEnt_Move_Left", true);
		pMonster->SetPos(LeftPos);
	}
	else if (MonPos.x <= InitPos.x - reconRange && !LeftEnd) // 정찰거리 왼쪽 끝에 도달하면 오른쪽으로 가게 해주기
	{
		LeftEnd = true;
		RightEnd = false;
	}
	else if (MonPos.x < InitPos.x + reconRange && !RightEnd)
	{
		CAnimator* MonAni = pMonster->GetMonsterAnimator();
		MonAni->Play(L"BEnt_Move_Right", true);
		pMonster->SetPos(RightPos);
	}
	else if (MonPos.x >= InitPos.x + reconRange && !RightEnd)
	{
		LeftEnd = false;
		RightEnd = true;
	}


	if (Distance < DetectRange)
	{
		if ((MonPos.y + MonScale.y / 2) - (PlePos.y + PleScale.y / 2) < 150.f && (PlePos.y + PleScale.y / 2) - (MonPos.y + MonScale.y / 2) < 150.f)
		{
			GetStateMachine()->ChangeState(L"TraceState");
		}

	}
}

void BEnt_Idle_State::Exit()
{
	//LOG(LOG_LEVEL::WARNING, L"Exit IdleState");
}
