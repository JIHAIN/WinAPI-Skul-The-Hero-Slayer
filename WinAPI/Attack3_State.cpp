#include "pch.h"
#include "Attack3_State.h"
#include "CAnimator.h"
#include "CKeyMgr.h"
#include "CSound.h"
#include "CAssetMgr.h"


Attack3_State::Attack3_State()
{
}

Attack3_State::~Attack3_State()
{
}


void Attack3_State::Enter()
{
	CPlayer* pPlayer = GetOwner<CPlayer>();

	CAnimator* PlaAni = pPlayer->GetPlayerAnimator();

	CSound* pBGM = CAssetMgr::Get()->LoadSound(L"Atk1", L"sound\\Atk_BluntWeapon_Large.wav");
	pBGM->Play();

	if (pPlayer->GetRightPrd() && pPlayer->GetPlayerprom())
	{
		PlaAni->Play(L"Dark_AttackA_Right", false);

		pPlayer->Dark_Atk(pPlayer->GetRightPrd());

		pPlayer->SetAttackCount(1);
	}
	else
	{
		PlaAni->Play(L"Dark_AttackA_Left", false);

		pPlayer->Dark_Atk(pPlayer->GetRightPrd());

		pPlayer->SetAttackCount(1);
	}

}

void Attack3_State::FinalTick()
{

}

void Attack3_State::Exit()
{

}