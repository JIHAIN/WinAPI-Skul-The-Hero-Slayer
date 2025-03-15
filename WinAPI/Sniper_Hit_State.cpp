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
	// ���͸� �˾Ƴ���.
	CMonster* pMonster = GetOwner<CMonster>();
	CAnimator* MonAni = pMonster->GetMonsterAnimator();

	// ���࿡ �νĹ��� ���� Player �� �־����� �ڽ��� ���¸� TraceState ���·� �����Ѵ�.
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