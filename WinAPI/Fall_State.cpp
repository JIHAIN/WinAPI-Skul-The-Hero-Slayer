#include "pch.h"
#include "Fall_State.h"

#include "CAnimator.h"
#include "CKeyMgr.h"

Fall_State::Fall_State()
{
}

Fall_State::~Fall_State()
{
}


void Fall_State::Enter()
{
	CPlayer* pPlayer = GetOwner<CPlayer>();

	CAnimator* PlaAni = pPlayer->GetPlayerAnimator();

	if (pPlayer->GetRightPrd() && !pPlayer->GetPlayerprom()) // 오른쪽 보고 있는지 
	{
		if (!pPlayer->GetHeadless())
		{
			PlaAni->Play(L"falling", true);
		}
		else
		{
			PlaAni->Play(L"Head_Fall_Right", true);
		}
	}
	else if (!pPlayer->GetRightPrd() && !pPlayer->GetPlayerprom())
	{
		if (!pPlayer->GetHeadless())
		{
			PlaAni->Play(L"falling_Left", true);
		}
		else
		{
			PlaAni->Play(L"Head_Fall_Left", true);
		}
	}
	else if (pPlayer->GetRightPrd() && pPlayer->GetPlayerprom())
	{
		PlaAni->Play(L"Dark_FallRepeat_Right", true);
	}
	else
	{
		PlaAni->Play(L"Dark_FallRepeat_Left", true);
	}



}

void Fall_State::FinalTick()
{
	CPlayer* pPlayer = GetOwner<CPlayer>();

	CAnimator* PlaAni = pPlayer->GetPlayerAnimator();

	if (pPlayer->GetGround())
	{
		if (pPlayer->GetRightPrd()) // 오른쪽 눌렀으면
		{
			GetStateMachine()->ChangeState(L"RightIdle");
		}
		else
		{
			GetStateMachine()->ChangeState(L"LeftIdle");
		}
		
	}
}

void Fall_State::Exit()
{

}