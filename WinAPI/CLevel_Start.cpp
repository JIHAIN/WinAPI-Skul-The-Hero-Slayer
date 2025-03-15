#include "pch.h"
#include "CLevel_Start.h"

#include "CEngine.h"

#include "CPlayer.h"
#include "CMonster.h"
#include "CPlatform.h"
#include "Cfloor.h"
#include "CTile.h"
#include "CTileMap.h"
#include "CBackground.h"
#include "CAttack.h"
#include "CTile.h"
#include "Door_1.h"

#include "BigEnt_Monster.h"
#include "Ent_Monster.h"
#include "BEnt_Monster.h"
#include "Sniper_Monster.h"
#include "Sword_Monster.h"
#include "Mage_Monster.h"
#include "Boss_Monster.h"
#include "Boss_Ball_Obj.h"
#include "Dark_Attack.h"

#include "CCamera.h"
#include "CLevelMgr.h"
#include "CAssetMgr.h"
#include "CSound.h"

#include "CCollisionMgr.h"
#include "CKeyMgr.h"

CLevel_Start::CLevel_Start()
{
}

CLevel_Start::~CLevel_Start()
{
}

void CLevel_Start::Init()
{	
	CObj* Cplaer = new CPlayer;
	CObj* CM1 = new BigEnt_Monster;
	CObj* CM2 = new Ent_Monster;
	CObj* CM3 = new BEnt_Monster;
	CObj* CM4 = new Sniper_Monster;
	CObj* CM5 = new Sword_Monster;
	CObj* CM6 = new Mage_Monster;
	CObj* CM7 = new Boss_Monster(Vec2(1360, 1400.f));
	CObj* CM8 = new Boss_Ball_Obj;
	CObj* CM9 = new Dark_Attack;



	CBackground* pBack1 = new CBackground;
	pBack1->SetPos(CCamera::Get()->GetLookAt());
	pBack1->SetTexture(L"Title_Art", L"texture\\Title_Art.png");
	AddObject(pBack1, LAYER_TYPE::BACKGROUND);

	CBackground* pBack2 = new CBackground;
	pBack2->SetPos(pBack1->GetPos().x, pBack1->GetPos().y + 400.f);
	pBack2->SetTexture(L"Title_Logo_White", L"texture\\Title_Logo_White.png");
	AddObject(pBack2, LAYER_TYPE::BACKGROUND);

	CSound* pBGM = CAssetMgr::Get()->LoadSound(L"Adventurer_Rockstar", L"sound\\Adventurer_Rockstar.wav");
	pBGM->PlayToBGM();

	delete Cplaer;
	delete CM1;
	delete CM2;
	delete CM3;
	delete CM4;
	delete CM5;
	delete CM6;
	delete CM7;
	delete CM8;
	delete CM9;
	
}

void CLevel_Start::Exit()
{
	// 모든 오브젝트를 삭제해둔다.
	DeleteAllObjects();

	CSound* pBGM = CAssetMgr::Get()->LoadSound(L"Adventurer_Rockstar", L"sound\\Adventurer_Rockstar.wav");
	pBGM->Stop();
	
}

void CLevel_Start::Tick()
{
	
	vector<CObj*> Backvec = this->GetLayer(LAYER_TYPE::BACKGROUND);
	CObj* Back = Backvec[0];
	Back->SetPos(CCamera::Get()->GetLookAt());

	CObj* Back1 = Backvec[1];
	Back1->SetPos(CCamera::Get()->GetLookAt());
	// 카메라 타겟 위치 설정 (플레이어 위치에서 약간 위쪽)


	
	

	// 마우스 클릭이 발생 시, 해당 위치에 Force 오브젝트 생성
	if (KEY_TAP(KEY::LBTN))
	{
		ChangeLevel(LEVEL_TYPE::STAGE_01);
		CCamera::Get()->SetFilterEffect(FADE_OUT, 0.5f);
		CCamera::Get()->SetFilterEffect(FADE_IN, 0.5f);
	}

	if (KEY_TAP(KEY::ENTER))
	{
		ChangeLevel(LEVEL_TYPE::EIDTOR);
		CCamera::Get()->SetFilterEffect(FADE_OUT, 0.5f);
		CCamera::Get()->SetFilterEffect(FADE_IN, 0.5f);
	}
}