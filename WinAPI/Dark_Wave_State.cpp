#include "pch.h"
#include "Dark_Wave_State.h"
#include "CAnimator.h"
#include "CKeyMgr.h"
#include "CSound.h"
#include "CAssetMgr.h"
#include "CTimeMgr.h"


Dark_Wave_State::Dark_Wave_State()
{
}

Dark_Wave_State::~Dark_Wave_State()
{
}


void Dark_Wave_State::Enter()
{
	CPlayer* pPlayer = GetOwner<CPlayer>();

	CAnimator* PlaAni = pPlayer->GetPlayerAnimator();

	CSound* pBGM = CAssetMgr::Get()->LoadSound(L"Atk1", L"sound\\Atk_Sword_Large.wav");
	pBGM->Play();


	if (pPlayer->GetRightPrd() && pPlayer->GetPlayerprom()) // 오른쪽 보고 있는지 
	{
		PlaAni->Play(L"Dark_SkillA_Right", false);
	}
	else if (!pPlayer->GetRightPrd() && pPlayer->GetPlayerprom())
	{
		PlaAni->Play(L"Dark_SkillA_Left", false);
	}


}

void Dark_Wave_State::FinalTick()
{

}

void Dark_Wave_State::Exit()
{

}