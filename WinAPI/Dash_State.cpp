#include "pch.h"
#include "Dash_State.h"

#include "CAnimator.h"
#include "CKeyMgr.h"

Dash_State::Dash_State()
{
}

Dash_State::~Dash_State()
{
}


void Dash_State::Enter()
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

void Dash_State::FinalTick()
{

}

void Dash_State::Exit()
{

}