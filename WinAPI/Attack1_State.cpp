#include "pch.h"
#include "Attack1_State.h"

#include "CAnimator.h"
#include "CKeyMgr.h"
#include "CSound.h"
#include "CAssetMgr.h"


Attack1_State::Attack1_State()
{
}

Attack1_State::~Attack1_State()
{
}


void Attack1_State::Enter()
{
	CPlayer* pPlayer = GetOwner<CPlayer>();

	CAnimator* PlaAni = pPlayer->GetPlayerAnimator();

	CSound* pBGM = CAssetMgr::Get()->LoadSound(L"Atk1", L"sound\\Atk_Sword_Large.wav");
	pBGM->Play();

	
	if (pPlayer->GetRightPrd() && !pPlayer->GetPlayerprom()) // 오른쪽 보고 있는지 
	{
		if (!pPlayer->GetHeadless())
		{
			PlaAni->Play(L"ATTACK1", false);
		}
		else
		{
			PlaAni->Play(L"Head_Attack1_Right", false);
		}

		pPlayer->Attack(pPlayer->GetRightPrd());

		pPlayer->SetAttackCount(2);
	}
	else if (!pPlayer->GetRightPrd() && !pPlayer->GetPlayerprom())
	{
		if (!pPlayer->GetHeadless())
		{
			PlaAni->Play(L"ATTACK1_Left", false);
		}
		else
		{
			PlaAni->Play(L"Head_Attack1_Left", false);
		}

		pPlayer->Attack(pPlayer->GetRightPrd());

		pPlayer->SetAttackCount(2);
	}
	else if (pPlayer->GetRightPrd() && pPlayer->GetPlayerprom())
	{
		PlaAni->Play(L"Dark_AttackC_Right", false);

		pPlayer->Dark_Atk(pPlayer->GetRightPrd());

		pPlayer->SetAttackCount(2);
	}
	else
	{
		PlaAni->Play(L"Dark_AttackC_Left", false);

		pPlayer->Dark_Atk(pPlayer->GetRightPrd());

		pPlayer->SetAttackCount(2);
	}

}

void Attack1_State::FinalTick()
{

}

void Attack1_State::Exit()
{

}