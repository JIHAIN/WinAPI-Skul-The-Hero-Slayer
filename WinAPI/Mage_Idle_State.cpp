#include "pch.h"
#include "Mage_Idle_State.h"

#include "CAnimator.h"
#include "CMonster.h"

Mage_Idle_State::Mage_Idle_State()
	: MoveOn(false)
	, RightEnd(false)
	, LeftEnd(false)
{
}

Mage_Idle_State::~Mage_Idle_State()
{
}

void Mage_Idle_State::Enter()
{
	// ���͸� �˾Ƴ���.
	CMonster* pMonster = GetOwner<CMonster>();

	

	//LOG(LOG_LEVEL::WARNING, L"Enter IdleState");
}

void Mage_Idle_State::FinalTick()
{
	// ������ �νĹ��� ���� Player �� �ִ��� Ȯ���Ѵ�.
	CMonster* pMonster = GetOwner<CMonster>();

	// ���࿡ �νĹ��� ���� Player �� �־����� �ڽ��� ���¸� TraceState ���·� �����Ѵ�.
	CPlayer* pPlayer = CLevelMgr::Get()->GetCurrentLevel()->GetPlayer();
	float Distance = pMonster->GetPos().Distance(pPlayer->GetPos());


	Vec2 MonPos = pMonster->GetPos();
	Vec2 MonScale = pMonster->GetScale();
	Vec2 PlePos = pPlayer->GetPos();
	Vec2 PleScale = pPlayer->GetScale();

	///////////////////////
	// Player ��ġ ��������.
	//CPlayer* pPlayer = CLevelMgr::Get()->GetCurrentLevel()->GetPlayer();

	//Vec2 MonPos = pMonster->GetPos();
	//Vec2 PlePos = pPlayer->GetPos();

	Vec2 vDir = PlePos - MonPos;
	vDir.Normalize();

	// ������ ���ؼ� �´� �ִϸ��̼� ���
	if (vDir > Vec2(0.f, 0.f))
	{
		CAnimator* MonAni = pMonster->GetMonsterAnimator();
		MonAni->Play(L"Mage_IDLE_Right", true);
	}
	else
	{
		CAnimator* MonAni = pMonster->GetMonsterAnimator();
		MonAni->Play(L"Mage_IDLE_Left", true);
	}

	////////////////////////////

	float DetectRange = pMonster->GetMonsterInfo().DetectRange;

	// DetecteRange ����� ������
	DrawDebugShape(DEBUG_SHAPE::CIRCLE, PEN_TYPE::BLUE, BRUSH_TYPE::HOLLOW
		, pMonster->GetRenderPos(), Vec2(DetectRange * 2.f, DetectRange * 2.f));


	
	if (Distance < DetectRange)
	{
		if ((MonPos.y + MonScale.y / 2) - (PlePos.y + PleScale.y / 2) < 300.f)
		{
			GetStateMachine()->ChangeState(L"TraceState");
		}

	}
}

void Mage_Idle_State::Exit()
{
	//LOG(LOG_LEVEL::WARNING, L"Exit IdleState");
}
