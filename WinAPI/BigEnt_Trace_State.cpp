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
	// Trace ������ ���͸� �˾Ƴ���.
	CMonster* pMonster = GetOwner<CMonster>();

	CAnimator* MonAni = pMonster->GetMonsterAnimator();
	MonAni->Play(L"BigEnt_LOW", true);

	AccTime = 0.f;

	//LOG(LOG_LEVEL::WARNING, L"Enter TraceState");
}

void BigEnt_Trace_State::FinalTick()
{
	// Trace ������ ���͸� �˾Ƴ���.
	CMonster* pMonster = GetOwner<CMonster>();

	// Player ��ġ ��������
	CPlayer* pPlayer = CLevelMgr::Get()->GetCurrentLevel()->GetPlayer();

	Vec2 MonPos = pMonster->GetPos();
	Vec2 MonScale = pMonster->GetScale();
	Vec2 PlePos = pPlayer->GetPos();
	Vec2 PleScale = pPlayer->GetScale();

	Vec2 vDir = pPlayer->GetPos() - MonPos;
	vDir.Normalize();

	// ���Ϳ� �÷��̾��� �Ÿ�
	float Distance = pMonster->GetPos().Distance(pPlayer->GetPos());

	// ���� ���ݰŸ�
	float AtkRange = pMonster->GetMonsterInfo().AttackRange;

	DrawDebugShape(DEBUG_SHAPE::CIRCLE, PEN_TYPE::BLUE, BRUSH_TYPE::HOLLOW
		, pMonster->GetRenderPos(), Vec2(AtkRange * 2.f, AtkRange * 2.f));

	AccTime += DT;

	// �÷��̾ ���ݰŸ� ���� �ٰ����� ����
	if (Distance < AtkRange && AccTime > pMonster->GetMonsterInfo().AtkCoolTime )
	{
		if (MonPos.y - PlePos.y <= 10.f)
		{
			GetStateMachine()->ChangeState(L"AttackState");
		}

	}

	// ���� Player �� Ž������ �̻����� �־����� 
	// or ������ ������ ���ķ� �����ð��� ���� ���
	// or �ʱ���ġ���� �����Ÿ� �̻� �߰��Ѱ��

	if (pMonster->GetMonsterInfo().DetectRange < Distance)
	{
		GetStateMachine()->ChangeState(L"IdleState");
	}
	


	
}

void BigEnt_Trace_State::Exit()
{
	//LOG(LOG_LEVEL::WARNING, L"Exit TraceState");
}
