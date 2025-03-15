#include "pch.h"
#include "Attack2_State.h"

#include "CAnimator.h"
#include "CKeyMgr.h"
#include "CSound.h"
#include "CAssetMgr.h"


Attack2_State::Attack2_State()
{
}

Attack2_State::~Attack2_State()
{
}


void Attack2_State::Enter()
{
	CPlayer* pPlayer = GetOwner<CPlayer>();

	CAnimator* PlaAni = pPlayer->GetPlayerAnimator();

	CSound* pBGM = CAssetMgr::Get()->LoadSound(L"Atk1", L"sound\\Atk_Sword_Small 3.wav");
	pBGM->Play();


	if (pPlayer->GetRightPrd() && !pPlayer->GetPlayerprom()) // 오른쪽 보고 있는지 
	{
		if (!pPlayer->GetHeadless())
		{
			PlaAni->Play(L"ATTACK2", false);
		}
		else
		{
			PlaAni->Play(L"Head_Attack2_Right", false);
		}

		pPlayer->Attack(pPlayer->GetRightPrd());

		pPlayer->SetAttackCount(1);
	}
	else if (!pPlayer->GetRightPrd() && !pPlayer->GetPlayerprom())
	{
		if (!pPlayer->GetHeadless())
		{
			PlaAni->Play(L"ATTACK2_Left", false);
		}
		else
		{
			PlaAni->Play(L"Head_Attack2_Left", false);
		}

		pPlayer->Attack(pPlayer->GetRightPrd());

		pPlayer->SetAttackCount(1);
	}
	else if (pPlayer->GetRightPrd() && pPlayer->GetPlayerprom())
	{
		PlaAni->Play(L"Dark_AttackB_Right", false);

		pPlayer->Dark_Atk(pPlayer->GetRightPrd());

		pPlayer->SetAttackCount(3);
	}
	else
	{
		PlaAni->Play(L"Dark_AttackB_Left", false);

		pPlayer->Dark_Atk(pPlayer->GetRightPrd());

		pPlayer->SetAttackCount(3);
	}

}

void Attack2_State::FinalTick()
{

}

void Attack2_State::Exit()
{

}