#include "pch.h"
#include "CEngine.h"

//레벨
#include "CLevelMgr.h"
#include "CLevel_Stage01.h"
#include "CLevel_Start.h"

//매니저
#include "CCamera.h"
#include "CKeyMgr.h"
#include "CDebugMgr.h"
#include "CLogMgr.h"
#include "CCollisionMgr.h"
#include "CAssetMgr.h"
#include "CTimeMgr.h"
#include "CPathMgr.h"
#include "CSound.h"

//오브젝트
#include "CPlayer.h"
#include "CMonster.h"
#include "CPlatform.h"
#include "Cfloor.h"
#include "CTile.h"
#include "CTileMap.h"
#include "CBackground.h"
#include "CAttack.h"
#include "CTile.h"
#include "CTileMap.h"
#include "Door_1.h"

#include "BigEnt_Monster.h"
#include "Ent_Monster.h"
#include "BEnt_Monster.h"
#include "Sniper_Monster.h"
#include "Sword_Monster.h"
#include "Mage_Monster.h"


#include "Player_State_UI.h"
#include "CPanel.h"



CLevel_Stage01::CLevel_Stage01()
{
}

CLevel_Stage01::~CLevel_Stage01()
{
}

void CLevel_Stage01::Init()
{
	Vec2 vResol = CEngine::Get()->GetResolution(); // 윈도우 해상도 받아오기

	// BGM 지정
	CSound* pBGM = CAssetMgr::Get()->LoadSound(L"Chapter1", L"sound\\Chapter1.wav");
	pBGM->PlayToBGM();


	float floorPick = 900;
	float ts = 64;

	

	// 타일사용법
	CTileMap* pTileMap = nullptr;

	// 플랫폼 설치
	wstring platformDataPath = CPathMgr::Get()->GetContentPath() + L"platform\\Stage1.map";
	LoadPlatformData(platformDataPath);
	CreatePlatformsFromData();

	CTile* pTile = new CTile;
	pTile->SetName(L"Tile Object");
	pTileMap = pTile->GetComponent<CTileMap>();
	pTileMap->UseLoad(L"Stage1_Tile");
	AddObject(pTile, LAYER_TYPE::TILE);

	// 오브젝트 위치 설정
	CPlayer* pPlayer = new CPlayer;
	pPlayer->SetName(L"Player");
	pPlayer->SetPos(320.f, 1280.f);
	Vec2 plaPos = pPlayer->GetPos();

	// 플레이어를 현재 레벨에 등록
	RegisterPlayer(pPlayer);
	AddObject(pPlayer, LAYER_TYPE::PLAYER);

	 
	///////// 테스트 몬스터 /////////////

	//CMonster* pObj = new Ent_Monster;
	//pObj->SetName(L"Monster");
	//pObj->SetPos(ts * 27, floorPick - 96.f);
	//pObj->SetInitPos(pObj->GetPos());
	//AddObject(pObj, LAYER_TYPE::MONSTER);
	
	CMonster* Bent = new BEnt_Monster;
	Bent->SetName(L"Monster");
	Bent->SetPos(2000, 360);
	Bent->SetInitPos(Bent->GetPos());
	AddObject(Bent, LAYER_TYPE::MONSTER);
	
	Bent = new BEnt_Monster;
	Bent->SetName(L"Monster");
	Bent->SetPos(2400, 360);
	Bent->SetInitPos(Bent->GetPos());
	AddObject(Bent, LAYER_TYPE::MONSTER);
	
	Bent = new BEnt_Monster;
	Bent->SetName(L"Monster");
	Bent->SetPos(2700, 360);
	Bent->SetInitPos(Bent->GetPos());
	AddObject(Bent, LAYER_TYPE::MONSTER);
	
	BigEnt_Monster* pBigEnt = new BigEnt_Monster;
	pBigEnt->SetName(L"Monster");
	pBigEnt->SetPos(1560,780);
	pBigEnt->SetInitPos(pBigEnt->GetPos());
	AddObject(pBigEnt, LAYER_TYPE::MONSTER);
	
	CMonster* pSword = new Sword_Monster;
	pSword->SetName(L"Monster");
	pSword->SetPos(1200.f, 1300.f);
	pSword->SetInitPos(pSword->GetPos());
	AddObject(pSword, LAYER_TYPE::MONSTER);
	
	pSword = new Sword_Monster;
	pSword->SetName(L"Monster");
	pSword->SetPos(1400.f, 1300.f);
	pSword->SetInitPos(pSword->GetPos());
	AddObject(pSword, LAYER_TYPE::MONSTER);
	
	pSword = new Sword_Monster;
	pSword->SetName(L"Monster");
	pSword->SetPos(1600.f, 1300.f);
	pSword->SetInitPos(pSword->GetPos());
	AddObject(pSword, LAYER_TYPE::MONSTER);
	
	pSword = new Sword_Monster;
	pSword->SetName(L"Monster");
	pSword->SetPos(1800.f, 1300.f);
	pSword->SetInitPos(pSword->GetPos());
	AddObject(pSword, LAYER_TYPE::MONSTER);

	/*Sniper_Monster* pSniper = new Sniper_Monster;
	pSniper->SetName(L"Monster");
	pSword->SetPos(1700.f, 1300.f);
	pSniper->SetInitPos(pSniper->GetPos());
	AddObject(pSniper, LAYER_TYPE::MONSTER);*/
	

	Mage_Monster* pObj = new Mage_Monster;
	pObj->SetName(L"Monster");
	pObj->SetPos(1400, 1030);
	pObj->SetInitPos(pObj->GetPos());
	AddObject(pObj, LAYER_TYPE::MONSTER);

	pObj = new Mage_Monster;
	pObj->SetName(L"Monster");
	pObj->SetPos(1600, 1030);
	pObj->SetInitPos(pObj->GetPos());
	AddObject(pObj, LAYER_TYPE::MONSTER);
	
	
	Door_1* Door = new Door_1;
	Door->SetPos(3400.f, 1540.f);
	AddObject(Door, LAYER_TYPE::DOOR);
	
	CBackground* pBack1 = new CBackground;
	pBack1->SetPos(1900, 500);
	pBack1->SetTexture(L"Background1", L"texture\\Background.png");
	AddObject(pBack1, LAYER_TYPE::BACKGROUND);

	// UI 장착
	Player_State_UI* PlUI = new Player_State_UI(Vec2(0.f, 650.f));
	PlUI->SetPos(0.f, 650.f);
	AddObject(PlUI, LAYER_TYPE::MYUI);


	// 카메라 범위 제한 지정
	CCamera::Get()->SetMapBounds(Vec2(0.f, 0.f), Vec2(3700.f, 3800.f));
	CCamera::Get()->SetLerpSpeedX(5.0f);
	CCamera::Get()->SetLerpSpeedY(20.0f);
	CCamera::Get()->SetDefaultPlayerY(0.6f);		// 플레이어가 화면 높이의 60% 위치에 있도록 설정
	CCamera::Get()->SetCameraAdjustThreshold(0.1f); // 플레이어가 화면 상단 30%에 진입하면 카메라 조정 시작
	CCamera::Get()->SetCameraAdjustSpeed(2.f);      // 카메라 조정 속도

	// 충돌 지정
	CCollisionMgr::Get()->CollisionCheck((UINT)LAYER_TYPE::PLAYER_PROJECTILE, (UINT)LAYER_TYPE::MONSTER, true);
	CCollisionMgr::Get()->CollisionCheck((UINT)LAYER_TYPE::PLAYER_PROJECTILE, (UINT)LAYER_TYPE::FLOOR, true);
	CCollisionMgr::Get()->CollisionCheck((UINT)LAYER_TYPE::PLAYER_PROJECTILE, (UINT)LAYER_TYPE::PLATFORM, true);
	CCollisionMgr::Get()->CollisionCheck((UINT)LAYER_TYPE::PLAYER, (UINT)LAYER_TYPE::PLATFORM, true);
	CCollisionMgr::Get()->CollisionCheck((UINT)LAYER_TYPE::PLAYER, (UINT)LAYER_TYPE::FLOOR, true);
	CCollisionMgr::Get()->CollisionCheck((UINT)LAYER_TYPE::PLAYER, (UINT)LAYER_TYPE::DOOR, true);
	CCollisionMgr::Get()->CollisionCheck((UINT)LAYER_TYPE::MONSTER, (UINT)LAYER_TYPE::PLATFORM, true);
	CCollisionMgr::Get()->CollisionCheck((UINT)LAYER_TYPE::MONSTER, (UINT)LAYER_TYPE::FLOOR, true);
	CCollisionMgr::Get()->CollisionCheck((UINT)LAYER_TYPE::PLAYER, (UINT)LAYER_TYPE::MONSTER, true);
}

void CLevel_Stage01::Exit()
{
	// 모든 오브젝트를 삭제해둔다.
	DeleteAllObjects();
}

void CLevel_Stage01::Tick()
{
	//필수
	CLevel::Tick();

	// 플레이어를 카메라가 따라다니게
	vector<CObj*> playervec = this->GetLayer(LAYER_TYPE::PLAYER);
	CObj* player = playervec[0];
	Vec2 pPos = player->GetPos();

	// 카메라 타겟 위치 설정 (플레이어 위치에서 약간 위쪽)
	CCamera::Get()->SetTargetPos(Vec2(pPos.x, pPos.y));

	// 맞춰서 배경내리기
	//vector<CObj*> Backvec = this->GetLayer(LAYER_TYPE::BACKGROUND);
	//for (int i = 0; Backvec.size() > i; ++i)
	//{
	//	CObj* Back = Backvec[i];
	//	Back->SetPos(Vec2(pPos.x, pPos.y - 250.f));
	//}
	
	Vec2 vMousePos = CKeyMgr::Get()->GetMousePos();
	Vec2 RealMousePos = CCamera::Get()->GetRealPos(vMousePos);

	wchar_t szMousPosLog[255] = {};
	if (KEY_TAP(KEY::RBTN))
	{
		swprintf_s(szMousPosLog, 255, L"TAP X : %d ,Y : %d ", (int)RealMousePos.x, (int)RealMousePos.y);
		MOUSELOG(LOG_LEVEL::WARNING, szMousPosLog);
	}

	if (KEY_TAP(KEY::ENTER))
	{
		ChangeLevel(LEVEL_TYPE::EIDTOR);
		CCamera::Get()->SetFilterEffect(FADE_OUT, 0.5f);
		CCamera::Get()->SetFilterEffect(FADE_IN, 0.5f);
	}

}
