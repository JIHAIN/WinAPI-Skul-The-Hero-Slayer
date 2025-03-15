#include "pch.h"
#include "Left_State.h"

#include "CAnimator.h"
#include "CKeyMgr.h"

Left_State::Left_State()
{
}

Left_State::~Left_State()
{
}


void Left_State::Enter()
{
	CPlayer* pPlayer = GetOwner<CPlayer>();

	CAnimator* PlaAni = pPlayer->GetPlayerAnimator();

	if (!pPlayer->GetHeadless() && !pPlayer->GetPlayerprom())
	{
		PlaAni->Play(L"MOVE_Left", true);
	}
	else if(pPlayer->GetHeadless() && !pPlayer->GetPlayerprom())
	{
		PlaAni->Play(L"Head_Move_Left", true);
	}

	else if (pPlayer->GetPlayerprom())
	{
		PlaAni->Play(L"Dark_Move_Left", true);
	}

}

void Left_State::FinalTick()
{

}

void Left_State::Exit()
{

}



