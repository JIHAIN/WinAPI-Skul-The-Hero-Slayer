#include "pch.h"
#include "Sword_Hit_State.h"
#include "CTimeMgr.h"
#include "CMonster.h"
#include "CAnimator.h"
#include "CRigidBody.h"

Sword_Hit_State::Sword_Hit_State()
{
}

Sword_Hit_State::~Sword_Hit_State()
{
}
void Sword_Hit_State::Enter()
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
		MonAni->Play(L"Sword_Hit_Left", false);
	}
	else
	{
		MonAni->Play(L"Sword_Hit_Right", false);
	}
}

void Sword_Hit_State::FinalTick()
{
	AccTime += DT;

	if (AccTime > 0.8f)
	{
		AccTime = 0.f;
		GetStateMachine()->ChangeState(L"IdleState");
		
	}

}

void Sword_Hit_State::Exit()
{
}