#include "pch.h"
#include "RightIdle_State.h"
#include "CAnimator.h"
#include "CTimeMgr.h"
#include "CKeyMgr.h"

void RightIdle_State::Enter()
{
	CPlayer* pPlayer = GetOwner<CPlayer>();

	CAnimator* PlaAni = pPlayer->GetPlayerAnimator();

	if (!pPlayer->GetHeadless() && !pPlayer->GetPlayerprom())
	{
		PlaAni->Play(L"IDLE_Right", true);
	}
	else if (pPlayer->GetHeadless() && !pPlayer->GetPlayerprom())
	{
		PlaAni->Play(L"Head_Idle_Right", true);
	}

	else if (pPlayer->GetPlayerprom())
	{
		PlaAni->Play(L"Dark_Idle_Right", true);
	}

}

void RightIdle_State::FinalTick()
{

}

void RightIdle_State::Exit()
{

}

RightIdle_State::RightIdle_State()
{
}

RightIdle_State::~RightIdle_State()
{
}