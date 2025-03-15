#include "pch.h"
#include "Right_State.h"

#include "CAnimator.h"
#include "CTimeMgr.h"
#include "CKeyMgr.h"

void Right_State::Enter()
{
	CPlayer* pPlayer = GetOwner<CPlayer>();

	CAnimator* PlaAni = pPlayer->GetPlayerAnimator();

	if (!pPlayer->GetHeadless() && !pPlayer->GetPlayerprom())
	{
		PlaAni->Play(L"MOVE_Right", true);
	}
	else if (pPlayer->GetHeadless() && !pPlayer->GetPlayerprom())
	{
		PlaAni->Play(L"Head_Move_Right", true);
	}

	else if (pPlayer->GetPlayerprom())
	{
		PlaAni->Play(L"Dark_Move_Right", true);
	}

}

void Right_State::FinalTick()
{

}

void Right_State::Exit()
{

}

Right_State::Right_State()
{
}

Right_State::~Right_State()
{
}

