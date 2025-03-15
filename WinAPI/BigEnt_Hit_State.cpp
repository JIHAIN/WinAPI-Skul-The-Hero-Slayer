#include "pch.h"
#include "BigEnt_Hit_State.h"
#include "CTimeMgr.h"
#include "CMonster.h"
#include "CAnimator.h"

BigEnt_Hit_State::BigEnt_Hit_State()
{
}

BigEnt_Hit_State::~BigEnt_Hit_State()
{
}
void BigEnt_Hit_State::Enter()
{
	// ���͸� �˾Ƴ���.
	CMonster* pMonster = GetOwner<CMonster>();
	CAnimator* MonAni = pMonster->GetMonsterAnimator();

	// ���࿡ �νĹ��� ���� Player �� �־����� �ڽ��� ���¸� TraceState ���·� �����Ѵ�.
	CPlayer* pPlayer = CLevelMgr::Get()->GetCurrentLevel()->GetPlayer();

	Vec2 MonPos = pMonster->GetPos();
	Vec2 PlePos = pPlayer->GetPos();


	if (MonPos.x < PlePos.x)
	{
	
	}
	else
	{
	
	}
}

void BigEnt_Hit_State::FinalTick()
{
	AccTime += DT;

	if (AccTime > 1.8f)
	{
		GetStateMachine()->ChangeState(L"IdleState");
	}

}

void BigEnt_Hit_State::Exit()
{
}
