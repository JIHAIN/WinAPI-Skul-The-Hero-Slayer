#include "pch.h"
#include "Ent_Hit_State.h"

#include "CTimeMgr.h"
#include "CMonster.h"
#include "CAnimator.h"
#include "CRigidBody.h"

Ent_Hit_State::Ent_Hit_State()
{
}

Ent_Hit_State::~Ent_Hit_State()
{
}
void Ent_Hit_State::Enter()
{
	// ���͸� �˾Ƴ���.
	CMonster* pMonster = GetOwner<CMonster>();
	CAnimator* MonAni = pMonster->GetMonsterAnimator();

	// ���࿡ �νĹ��� ���� Player �� �־����� �ڽ��� ���¸� TraceState ���·� �����Ѵ�.
	CPlayer* pPlayer = CLevelMgr::Get()->GetCurrentLevel()->GetPlayer();

	Vec2 MonPos = pMonster->GetPos();
	Vec2 PlePos = pPlayer->GetPos();
	CRigidBody* MonRig = pMonster->GetRigid();
	Vec2 Dir = Vec2(1.f, 0.f);
	MonRig->SetVelocity(-Dir * 200.f * DT);

	if (MonPos.x > PlePos.x)
	{
		MonAni->Play(L"Ent_Hit_Left", false);
	}
	else
	{
		MonAni->Play(L"Ent_Hit_Right", false);
	}
}

void Ent_Hit_State::FinalTick()
{
	AccTime += DT;

	if (AccTime > 0.8f)
	{
		AccTime = 0.f;
		GetStateMachine()->ChangeState(L"IdleState");
	}

}

void Ent_Hit_State::Exit()
{
}