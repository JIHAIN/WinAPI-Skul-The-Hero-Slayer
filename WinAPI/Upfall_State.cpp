#include "pch.h"
#include "Upfall_State.h"

#include "CAnimator.h"
#include "CKeyMgr.h"

Upfall_State::Upfall_State()
{
}

Upfall_State::~Upfall_State()
{
}


void Upfall_State::Enter()
{
	CPlayer* pPlayer = GetOwner<CPlayer>();

	CAnimator* PlaAni = pPlayer->GetPlayerAnimator();

	if (pPlayer->GetHeadless())
	{
		PlaAni->Play(L"MOVE_Right", true);
	}
	else
	{
		PlaAni->Play(L"Head_Move_Right", true);
	}

}

void Upfall_State::FinalTick()
{
	

	
}

void Upfall_State::Exit()
{

}
