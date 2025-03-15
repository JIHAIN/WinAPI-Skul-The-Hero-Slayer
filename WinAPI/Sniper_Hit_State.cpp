#include "pch.h"
#include "Sniper_Hit_State.h"

#include "CTimeMgr.h"
#include "CMonster.h"
#include "CAnimator.h"
#include "CRigidBody.h"

Sniper_Hit_State::Sniper_Hit_State()
{
}

Sniper_Hit_State::~Sniper_Hit_State()
{
}
void Sniper_Hit_State::Enter()
{
	// 몬스터를 알아낸다.
	CMonster* pMonster = GetOwner<CMonster>();
	CAnimator* MonAni = pMonster->GetMonsterAnimator();

	// 만약에 인식범위 내에 Player 가 있었으면 자신의 상태를 TraceState 상태로 변경한다.
	CPlayer* pPlayer = CLevelMgr::Get()->GetCurrentLevel()->GetPlayer();

	Vec2 MonPos = pMonster->GetPos();
	Vec2 PlePos = pPlayer->GetPos();
	

	if (MonPos.x > PlePos.x)
	{
		MonAni->Play(L"Sniper_Hit_Left", true);
	}
	else
	{
		MonAni->Play(L"Sniper_Hit_Right", true);
	}
}

void Sniper_Hit_State::FinalTick()
{
	AccTime += DT;

	if (AccTime > 0.8f)
	{
		AccTime = 0.f;
		GetStateMachine()->ChangeState(L"IdleState");
	}

}

void Sniper_Hit_State::Exit()
{
}