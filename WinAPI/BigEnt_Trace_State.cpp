#include "pch.h"
#include "BigEnt_Trace_State.h"

#include "CMonster.h"
#include "CAnimator.h"
#include "CTimeMgr.h"

BigEnt_Trace_State::BigEnt_Trace_State()
{
}

BigEnt_Trace_State::~BigEnt_Trace_State()
{
}

void BigEnt_Trace_State::Enter()
{
	// Trace 상태의 몬스터를 알아낸다.
	CMonster* pMonster = GetOwner<CMonster>();

	CAnimator* MonAni = pMonster->GetMonsterAnimator();
	MonAni->Play(L"BigEnt_LOW", true);

	AccTime = 0.f;

	//LOG(LOG_LEVEL::WARNING, L"Enter TraceState");
}

void BigEnt_Trace_State::FinalTick()
{
	// Trace 상태의 몬스터를 알아낸다.
	CMonster* pMonster = GetOwner<CMonster>();

	// Player 위치 가져오기
	CPlayer* pPlayer = CLevelMgr::Get()->GetCurrentLevel()->GetPlayer();

	Vec2 MonPos = pMonster->GetPos();
	Vec2 MonScale = pMonster->GetScale();
	Vec2 PlePos = pPlayer->GetPos();
	Vec2 PleScale = pPlayer->GetScale();

	Vec2 vDir = pPlayer->GetPos() - MonPos;
	vDir.Normalize();

	// 몬스터와 플레이어의 거리
	float Distance = pMonster->GetPos().Distance(pPlayer->GetPos());

	// 몬스터 공격거리
	float AtkRange = pMonster->GetMonsterInfo().AttackRange;

	DrawDebugShape(DEBUG_SHAPE::CIRCLE, PEN_TYPE::BLUE, BRUSH_TYPE::HOLLOW
		, pMonster->GetRenderPos(), Vec2(AtkRange * 2.f, AtkRange * 2.f));

	AccTime += DT;

	// 플레이어가 공격거리 까지 다가가면 공격
	if (Distance < AtkRange && AccTime > pMonster->GetMonsterInfo().AtkCoolTime )
	{
		if (MonPos.y - PlePos.y <= 10.f)
		{
			GetStateMachine()->ChangeState(L"AttackState");
		}

	}

	// 만약 Player 가 탐지범위 이상으로 멀어지면 
	// or 추적을 시작한 이후로 일정시간이 지난 경우
	// or 초기위치에서 일정거리 이상 추격한경우

	if (pMonster->GetMonsterInfo().DetectRange < Distance)
	{
		GetStateMachine()->ChangeState(L"IdleState");
	}
	


	
}

void BigEnt_Trace_State::Exit()
{
	//LOG(LOG_LEVEL::WARNING, L"Exit TraceState");
}
