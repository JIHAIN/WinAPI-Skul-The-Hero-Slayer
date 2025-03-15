#include "pch.h"
#include "Jump_State.h"

#include "CAnimator.h"
#include "CKeyMgr.h"

Jump_State::Jump_State()
{
}

Jump_State::~Jump_State()
{
}


void Jump_State::Enter()
{
	CPlayer* pPlayer = GetOwner<CPlayer>();

	CAnimator* PlaAni = pPlayer->GetPlayerAnimator();

	if (pPlayer->GetRightPrd() && !pPlayer->GetPlayerprom()) // 오른쪽 보고 있는지 
	{
		if (!pPlayer->GetHeadless())
		{
			PlaAni->Play(L"UP", true);
		}
		else
		{
			PlaAni->Play(L"Head_Up_Right", true);
		}
	}
	else if(!pPlayer->GetRightPrd() && !pPlayer->GetPlayerprom())
	{
		if (!pPlayer->GetHeadless())
		{
			PlaAni->Play(L"UP_Left", true);
		}
		else
		{
			PlaAni->Play(L"Head_Up_Left", true);
		}
	}
	else if(pPlayer->GetRightPrd() && pPlayer->GetPlayerprom())
	{
		PlaAni->Play(L"Dark_Jump_Right", true);
	}
	else
	{
		PlaAni->Play(L"Dark_Jump_Left", true);
	}

}

void Jump_State::FinalTick()
{

}

void Jump_State::Exit()
{

}
