#include "pch.h"
#include "Sword_Idle_State.h"

#include "CAnimator.h"
#include "CMonster.h"

Sword_Idle_State::Sword_Idle_State()
	: MoveOn(false)
	, RightEnd(false)
	, LeftEnd(false)
{
}

Sword_Idle_State::~Sword_Idle_State()
{
	


}

void Sword_Idle_State::Enter()
{
	
	//CMonster* pMonster = GetOwner<CMonster>();
	//CAnimator* MonAni = pMonster->GetMonsterAnimator();
	//
	//Vec2 PrevPos = pMonster->GetPrevPos();
	//Vec2 CurvPos = pMonster->GetPos();
	//
	//if (PrevPos.x < CurvPos.x)
	//{
	//	MonAni->Play(L"Sword_Move_Right", true);
	//}
	//else
	//{
	//	MonAni->Play(L"Sword_Move_Left", true);
	//}
}

void Sword_Idle_State::FinalTick()
{
	
	CMonster* pMonster = GetOwner<CMonster>();

	float DetectRange = pMonster->GetMonsterInfo().DetectRange;

	// DetecteRange ����� ������
	DrawDebugShape(DEBUG_SHAPE::CIRCLE, PEN_TYPE::BLUE, BRUSH_TYPE::HOLLOW
		, pMonster->GetRenderPos(), Vec2(DetectRange * 2.f, DetectRange * 2.f));


	// ���࿡ �νĹ��� ���� Player �� �־����� �ڽ��� ���¸� TraceState ���·� �����Ѵ�.
	CPlayer* pPlayer = CLevelMgr::Get()->GetCurrentLevel()->GetPlayer();

	float Distance = pMonster->GetPos().Distance(pPlayer->GetPos());

	Vec2 MonPos = pMonster->GetPos();
	Vec2 MonScale = pMonster->GetScale();
	Vec2 PlePos = pPlayer->GetPos();
	Vec2 PleScale = pPlayer->GetScale();

	Vec2 InitPos = pMonster->GetMonsterInfo().InitPos;			// ó����ġ
	float reconRange = pMonster->GetMonsterInfo().reconRange;	// �����Ÿ�

	Vec2 RightPos = pMonster->GetPos() + Vec2(1.f, 0.f) * pMonster->GetMonsterInfo().Speed * DT; // ���������� �����̰� ����
	Vec2 LeftPos = pMonster->GetPos() + Vec2(-1.f, 0.f) * pMonster->GetMonsterInfo().Speed * DT; // �������� �����̰� ����


	if (MonPos.x > InitPos.x - reconRange && !LeftEnd)  // �� ���� ��ġ�� ���� �����Ÿ� ��ŭ �������� üũ.
	{
		CAnimator* MonAni = pMonster->GetMonsterAnimator();
		MonAni->Play(L"Sword_Move_Left", true);
		pMonster->SetPos(LeftPos);
	}
	else if (MonPos.x <= InitPos.x - reconRange && !LeftEnd) // �����Ÿ� ���� ���� �����ϸ� ���������� ���� ���ֱ�
	{
		LeftEnd = true;
		RightEnd = false;
	}
	else if (MonPos.x < InitPos.x + reconRange && !RightEnd)
	{
		CAnimator* MonAni = pMonster->GetMonsterAnimator();
		MonAni->Play(L"Sword_Move_Right", true);
		pMonster->SetPos(RightPos);
	}
	else if (MonPos.x >= InitPos.x + reconRange && !RightEnd)
	{
		LeftEnd = false;
		RightEnd = true;
	}



	if (Distance < DetectRange)
	{
		if ((MonPos.y + MonScale.y / 2) - (PlePos.y + PleScale.y / 2) < 150.f &&  (PlePos.y + PleScale.y / 2) - (MonPos.y + MonScale.y / 2) < 150.f)
		{
			GetStateMachine()->ChangeState(L"TraceState");
		}

	}
}

void Sword_Idle_State::Exit()
{
	//LOG(LOG_LEVEL::WARNING, L"Exit IdleState");
}

