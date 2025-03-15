#include "pch.h"
#include "BEnt_Trace_State.h"

#include "CMonster.h"
#include "CAnimator.h"
#include "CTimeMgr.h"

BEnt_Trace_State::BEnt_Trace_State()
	:AccTime (0.f)
{
}

BEnt_Trace_State::~BEnt_Trace_State()
{
}

void BEnt_Trace_State::Enter()
{
	AccTime = 0.f;
	//LOG(LOG_LEVEL::WARNING, L"Enter TraceState");
}

void BEnt_Trace_State::FinalTick()
{

		// Trace 상태의 몬스터를 알아낸다.
		CMonster* pMonster = GetOwner<CMonster>();

		// Player 를 추적한다.
		CPlayer* pPlayer = CLevelMgr::Get()->GetCurrentLevel()->GetPlayer();

		Vec2 MonPos = pMonster->GetPos();
		Vec2 MonScale = pMonster->GetScale();
		Vec2 PlePos = pPlayer->GetPos();
		Vec2 PleScale = pPlayer->GetScale();

		Vec2 vDir = pPlayer->GetPos() - MonPos;
		vDir.Normalize();

		Vec2 vPos = pMonster->GetPos() + vDir * pMonster->GetMonsterInfo().Speed * DT;
		pMonster->SetPos(vPos.x, MonPos.y);

		if (vDir.x > 0)
		{
			CAnimator* MonAni = pMonster->GetMonsterAnimator();
			MonAni->Play(L"BEnt_Move_Right", true);
		}
		else
		{
			CAnimator* MonAni = pMonster->GetMonsterAnimator();
			MonAni->Play(L"BEnt_Move_Left", true);
		}

		float Distance = pMonster->GetPos().Distance(pPlayer->GetPos());

		float AtkRange = pMonster->GetMonsterInfo().AttackRange;

		DrawDebugShape(DEBUG_SHAPE::CIRCLE, PEN_TYPE::BLUE, BRUSH_TYPE::HOLLOW
			, pMonster->GetRenderPos(), Vec2(AtkRange * 2.f, AtkRange * 2.f));

		AccTime += DT;

		// 몬스터가 공격거리 까지 다가가면 공격
		if (Distance < AtkRange && AccTime > pMonster->GetMonsterInfo().AtkCoolTime)
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

void BEnt_Trace_State::Exit()
{

}