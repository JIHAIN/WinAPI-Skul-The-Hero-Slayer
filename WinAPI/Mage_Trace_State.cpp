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
	// ���͸� �˾Ƴ���.
	CMonster* pMonster = GetOwner<CMonster>();

	// Player ��ġ ��������.
	CPlayer* pPlayer = CLevelMgr::Get()->GetCurrentLevel()->GetPlayer();

	Vec2 MonPos = pMonster->GetPos();
	Vec2 PlePos = pPlayer->GetPos();

	Vec2 vDir = PlePos - MonPos;
	vDir.Normalize();

	AccTime = 0.f;

	CSound* pBGM = CAssetMgr::Get()->LoadSound(L"Wizard_Charge_Loop", L"sound\\Wizard_Charge_Loop.wav");
	pBGM->Play();

	// ������ ���ؼ� �´� �ִϸ��̼� ���
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
	if (Distance < AtkRange && AccTime > pMonster->GetMonsterInfo().AtkCoolTime)
	{
	 GetStateMachine()->ChangeState(L"AttackState");
	}

	// ���� Player �� Ž������ �̻����� �־����� 
	// or ������ ������ ���ķ� �����ð��� ���� ���
	// or �ʱ���ġ���� �����Ÿ� �̻� �߰��Ѱ��

	if (pMonster->GetMonsterInfo().DetectRange < Distance)
	{
		GetStateMachine()->ChangeState(L"IdleState");
	}




}

void Mage_Trace_State::Exit()
{
	//LOG(LOG_LEVEL::WARNING, L"Exit TraceState");
}