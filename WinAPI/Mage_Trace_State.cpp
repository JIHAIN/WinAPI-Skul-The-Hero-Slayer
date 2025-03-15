#include "pch.h"
#include "Mage_Trace_State.h"

#include "CMonster.h"
#include "CAnimator.h"
#include "CTimeMgr.h"

Mage_Trace_State::Mage_Trace_State()
	: AccTime(0.f)
{
}

Mage_Trace_State::~Mage_Trace_State()
{
}

void Mage_Trace_State::Enter()
{
	// 몬스터를 알아낸다.
	CMonster* pMonster = GetOwner<CMonster>();

	// Player 위치 가져오기.
	CPlayer* pPlayer = CLevelMgr::Get()->GetCurrentLevel()->GetPlayer();

	Vec2 MonPos = pMonster->GetPos();
	Vec2 PlePos = pPlayer->GetPos();

	Vec2 vDir = PlePos - MonPos;
	vDir.Normalize();

	AccTime = 0.f;

	CSound* pBGM = CAssetMgr::Get()->LoadSound(L"Wizard_Charge_Loop", L"sound\\Wizard_Charge_Loop.wav");
	pBGM->Play();

	// 방향을 구해서 맞는 애니메이션 재생
	if (vDir.x > 0)
	{
		CAnimator* MonAni = pMonster->GetMonsterAnimator();
		MonAni->Play(L"Mage_IDLE_Right", true);
	}
	else
	{
		CAnimator* MonAni = pMonster->GetMonsterAnimator();
		MonAni->Play(L"Mage_IDLE_Left", true);
	}
}

void Mage_Trace_State::FinalTick()
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
	if (Distance < AtkRange && AccTime > pMonster->GetMonsterInfo().AtkCoolTime)
	{
	 GetStateMachine()->ChangeState(L"AttackState");
	}

	// 만약 Player 가 탐지범위 이상으로 멀어지면 
	// or 추적을 시작한 이후로 일정시간이 지난 경우
	// or 초기위치에서 일정거리 이상 추격한경우

	if (pMonster->GetMonsterInfo().DetectRange < Distance)
	{
		GetStateMachine()->ChangeState(L"IdleState");
	}




}

void Mage_Trace_State::Exit()
{
	//LOG(LOG_LEVEL::WARNING, L"Exit TraceState");
}