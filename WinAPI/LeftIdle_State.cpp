#include "pch.h"
#include "LeftIdle_State.h"
#include "CAnimator.h"
#include "CTimeMgr.h"
#include "CKeyMgr.h"

void LeftIdle_State::Enter()
{
	CPlayer* pPlayer = GetOwner<CPlayer>();

	CAnimator* PlaAni = pPlayer->GetPlayerAnimator();

	if (!pPlayer->GetHeadless() && !pPlayer->GetPlayerprom())
	{
		PlaAni->Play(L"IDLE_Left", true);
	}
	else if (pPlayer->GetHeadless() && !pPlayer->GetPlayerprom())
	{
		PlaAni->Play(L"Head_Idle_Left", true);
	}

	else if (pPlayer->GetPlayerprom())
	{
		PlaAni->Play(L"Dark_Idle_Left", true);
	}
}

void LeftIdle_State::FinalTick()
{

}

void LeftIdle_State::Exit()
{

}

LeftIdle_State::LeftIdle_State()
{
}

LeftIdle_State::~LeftIdle_State()
{
}