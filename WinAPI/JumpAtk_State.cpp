#include "pch.h"
#include "JumpAtk_State.h"

#include "CAnimator.h"
#include "CKeyMgr.h"

JumpAtk_State::JumpAtk_State()
{
}

JumpAtk_State::~JumpAtk_State()
{
}


void JumpAtk_State::Enter()
{
	CPlayer* pPlayer = GetOwner<CPlayer>();

	CAnimator* PlaAni = pPlayer->GetPlayerAnimator();

	if (pPlayer->GetRightPrd()) // 오른쪽 보고 있는지 
	{
		if (!pPlayer->GetHeadless())
		{
			PlaAni->Play(L"Jump_Atk_Right", false);
		}
		else
		{
			PlaAni->Play(L"Head_JumpAtk_Right", false);
		}

		pPlayer->Attack(pPlayer->GetRightPrd());

		pPlayer->SetAttackCount(1);
	}
	else
	{
		if (!pPlayer->GetHeadless())
		{
			PlaAni->Play(L"Jump_Atk_Left", false);
		}
		else
		{
			PlaAni->Play(L"Head_JumpAtk_Left", false);
		}

		pPlayer->Attack(pPlayer->GetRightPrd());

		pPlayer->SetAttackCount(1);

	}
}

void JumpAtk_State::FinalTick()
{

}

void JumpAtk_State::Exit()
{

}