#include "pch.h"
#include "BigEnt_Idle_State.h"

#include "CAnimator.h"
#include "CMonster.h"

BigEnt_Idle_State::BigEnt_Idle_State()
	: MoveOn(false)
	, RightEnd(false)
	, LeftEnd(false)
{
}

BigEnt_Idle_State::~BigEnt_Idle_State()
{
}

void BigEnt_Idle_State::Enter()
{
	//LOG(LOG_LEVEL::WARNING, L"Enter IdleState");
}

void BigEnt_Idle_State::FinalTick()
{

	// 몬스터를 알아낸다.
	CMonster* pMonster = GetOwner<CMonster>();
	CAnimator* MonAni = pMonster->GetMonsterAnimator();
	MonAni->Play(L"BigEnt_IDLE", true);

	

	// 몬스터의 인식범위 내에 Player 가 있는지 확인한다.
	//CMonster* pMonster = GetOwner<CMonster>();

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


	if (Distance < DetectRange)
	{
		if ((MonPos.y + MonScale.y / 2) - (PlePos.y + PleScale.y / 2) < 300.f)
		{
			GetStateMachine()->ChangeState(L"TraceState");
		}

	}
}

void BigEnt_Idle_State::Exit()
{
	//LOG(LOG_LEVEL::WARNING, L"Exit IdleState");
}
