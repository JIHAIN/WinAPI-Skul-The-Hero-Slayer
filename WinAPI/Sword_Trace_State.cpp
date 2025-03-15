#include "pch.h"
#include "Sword_Trace_State.h"

#include "CMonster.h"
#include "CAnimator.h"
#include "CTimeMgr.h"

Sword_Trace_State::Sword_Trace_State()
	:AccTime(0.f)
{
}

Sword_Trace_State::~Sword_Trace_State()
{
}

void Sword_Trace_State::Enter()
{

	AccTime = 0.f;
	//LOG(LOG_LEVEL::WARNING, L"Enter TraceState");
}

void Sword_Trace_State::FinalTick()
{

	
		// Trace ������ ���͸� �˾Ƴ���.
		CMonster* pMonster = GetOwner<CMonster>();

		// Player �� �����Ѵ�.
		CPlayer* pPlayer = CLevelMgr::Get()->GetCurrentLevel()->GetPlayer();

		Vec2 MonPos = pMonster->GetPos();
		Vec2 MonScale = pMonster->GetScale();
		Vec2 PlePos = pPlayer->GetPos();
		Vec2 PleScale = pPlayer->GetScale();

		Vec2 vDir = pPlayer->GetPos() - MonPos;
		vDir.Normalize();

		Vec2 vPos = pMonster->GetPos() + vDir * pMonster->GetMonsterInfo().Speed * 1.5f * DT;
		pMonster->SetPos(vPos.x, MonPos.y);

		if (vDir.x > 0)
		{
			CAnimator* MonAni = pMonster->GetMonsterAnimator();
			MonAni->Play(L"Sword_Move_Right", true);
		}
		else
		{
			CAnimator* MonAni = pMonster->GetMonsterAnimator();
			MonAni->Play(L"Sword_Move_Left", true);
		}

		float Distance = pMonster->GetPos().Distance(pPlayer->GetPos());

		float AtkRange = pMonster->GetMonsterInfo().AttackRange;

		DrawDebugShape(DEBUG_SHAPE::CIRCLE, PEN_TYPE::BLUE, BRUSH_TYPE::HOLLOW
			, pMonster->GetRenderPos(), Vec2(AtkRange * 2.f, AtkRange * 2.f));


		AccTime += DT;

		// ���Ͱ� ���ݰŸ� ���� �ٰ����� ����
		if (Distance < AtkRange && AccTime > pMonster->GetMonsterInfo().AtkCoolTime)
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
	

	//else
	//{
	//	// ���� ��ġ�� ���ư���.
	//	CMonster* pMonster = GetOwner<CMonster>();
	//	m_InitPos = pMonster->GetMonsterInfo().InitPos;
	//
	//	Vec2 MonPos = pMonster->GetPos();
	//	Vec2 vDir = m_InitPos - MonPos;
	//	vDir.Normalize();
	//	Vec2 vNewPos = pMonster->GetPos() + vDir * pMonster->GetMonsterInfo().Speed * 2.f * DT;
	//
	//	if (vDir.x > 0)
	//	{
	//		CAnimator* MonAni = pMonster->GetMonsterAnimator();
	//		MonAni->Play(L"Sword_Move_Right", true);
	//	}
	//	else
	//	{
	//		CAnimator* MonAni = pMonster->GetMonsterAnimator();
	//		MonAni->Play(L"Sword_Move_Left", true);
	//	}
	//
	//	pMonster->SetPos(vNewPos);
	//
	//	float Distance = pMonster->GetPos().Distance(m_InitPos);
	//
	//	// �ʱ���ġ�� ������ �ǵ��ƿ��� IdleState �� ����
	//	if (Distance <= 1.f)
	//	{
	//		pMonster->SetPos(m_InitPos);
	//		GetStateMachine()->ChangeState(L"IdleState");
	//	}
	//}
}

void Sword_Trace_State::Exit()
{
}