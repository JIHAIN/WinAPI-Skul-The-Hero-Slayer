#include "pch.h"
#include "CEngine.h"
#include "CLevel_Stage05.h"

//����
#include "CLevelMgr.h"
#include "CLevel_Stage01.h"
#include "CLevel_Start.h"

//�Ŵ���
#include "CCamera.h"
#include "CKeyMgr.h"
#include "CDebugMgr.h"
#include "CLogMgr.h"
#include "CCollisionMgr.h"
#include "CAssetMgr.h"
#include "CTimeMgr.h"
#include "CPathMgr.h"
#include "CSound.h"

//������Ʈ
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
#include "Player_State_UI.h"

#include "Boss_Monster.h"
#include "Boss_Ball_Create.h"
#include "Boss_Name.h"


CLevel_Stage05::CLevel_Stage05()
{
}

CLevel_Stage05::~CLevel_Stage05()
{
}

void CLevel_Stage05::Init()
{
	Vec2 vResol = CEngine::Get()->GetResolution(); // ������ �ػ� �޾ƿ���

	// Ÿ�ϻ���
	CTileMap* pTileMap = nullptr;

	// �÷��� ��ġ
	wstring platformDataPath = CPathMgr::Get()->GetContentPath() + L"platform\\Stage5.map";
	LoadPlatformData(platformDataPath);
	CreatePlatformsFromData();

	CSound* m_Sound = CAssetMgr::Get()->LoadSound(L"Chapter1_Boss", L"sound\\Chapter1_Boss.wav");
	m_Sound->PlayToBGM(true);

	CTile* pTile = new CTile;
	pTile->SetName(L"Tile Object");
	pTileMap = pTile->GetComponent<CTileMap>();
	pTileMap->UseLoad(L"Stage5_Tile");
	AddObject(pTile, LAYER_TYPE::TILE);

	// ������Ʈ ��ġ ����
	CPlayer* pPlayer = new CPlayer;
	pPlayer->SetName(L"Player");
	pPlayer->SetPos(1020.f, 650.f);
	Vec2 plaPos = pPlayer->GetPos();

	// �÷��̾ ���� ������ ���
	RegisterPlayer(pPlayer);
	AddObject(pPlayer, LAYER_TYPE::PLAYER);



	Boss_Monster* pObj = new Boss_Monster(Vec2(1360,1400.f)); // �Ӹ��� �߽����� �ٸ� �������� ���õ� ��
	pObj->SetName(L"Monster");
	pObj->SetPos(1360, 1400.f - 80.f); // �Ӹ��� �ð�
	pObj->SetInitPos(pObj->GetPos());

	// ���� ���
	RegisterBoss(pObj);
	AddObject(pObj, LAYER_TYPE::MONSTER);

	CBackground* pBack1 = new CBackground;
	pBack1->SetPos(1360, 1200);
	pBack1->SetTexture(L"Background", L"texture\\BossMap12.png");
	AddObject(pBack1, LAYER_TYPE::BACKGROUND);

	// UI ����
	Player_State_UI* PlUI = new Player_State_UI(Vec2(0.f, 650.f));
	PlUI->SetPos(0.f, 650.f);
	AddObject(PlUI, LAYER_TYPE::MYUI);

	Boss_Name* BUI = new Boss_Name(Vec2(310.f, 0.f));
	BUI->SetPos(300.f, 0.f);
	AddObject(BUI, LAYER_TYPE::MYUI);


	// ī�޶� ���� ���� ����
	CCamera::Get()->SetMapBounds(Vec2(0.f,565.f), Vec2(2720.f, 2000.f));
	CCamera::Get()->SetLerpSpeedX(5.0f);
	CCamera::Get()->SetLerpSpeedY(20.0f);
	CCamera::Get()->SetDefaultPlayerY(0.6f);		// �÷��̾ ȭ�� ������ 60% ��ġ�� �ֵ��� ����
	CCamera::Get()->SetCameraAdjustThreshold(0.1f); // �÷��̾ ȭ�� ��� 30%�� �����ϸ� ī�޶� ���� ����
	CCamera::Get()->SetCameraAdjustSpeed(2.f);      // ī�޶� ���� �ӵ�

	// �浹 ����
	CCollisionMgr::Get()->CollisionCheck((UINT)LAYER_TYPE::PLAYER_PROJECTILE, (UINT)LAYER_TYPE::MONSTER, true);
	CCollisionMgr::Get()->CollisionCheck((UINT)LAYER_TYPE::PLAYER_PROJECTILE, (UINT)LAYER_TYPE::FLOOR, true);
	CCollisionMgr::Get()->CollisionCheck((UINT)LAYER_TYPE::PLAYER_PROJECTILE, (UINT)LAYER_TYPE::PLATFORM, true);
	CCollisionMgr::Get()->CollisionCheck((UINT)LAYER_TYPE::PLAYER, (UINT)LAYER_TYPE::PLATFORM, true);
	CCollisionMgr::Get()->CollisionCheck((UINT)LAYER_TYPE::PLAYER, (UINT)LAYER_TYPE::FLOOR, true);
	CCollisionMgr::Get()->CollisionCheck((UINT)LAYER_TYPE::MONSTER, (UINT)LAYER_TYPE::PLATFORM, true);
	CCollisionMgr::Get()->CollisionCheck((UINT)LAYER_TYPE::PLAYER, (UINT)LAYER_TYPE::MONSTER, true);
}

void CLevel_Stage05::Exit()
{
	// ��� ������Ʈ�� �����صд�.
	DeleteAllObjects();
}

void CLevel_Stage05::Tick()
{
	//�ʼ�
	CLevel::Tick();

	// �÷��̾ ī�޶� ����ٴϰ�
	vector<CObj*> playervec = this->GetLayer(LAYER_TYPE::PLAYER);
	CObj* player = playervec[0];
	Vec2 pPos = player->GetPos();

	// ī�޶� Ÿ�� ��ġ ���� (�÷��̾� ��ġ���� �ణ ����)
	CCamera::Get()->SetTargetPos(Vec2(pPos.x, pPos.y));

	Vec2 vMousePos = CKeyMgr::Get()->GetMousePos();
	Vec2 RealMousePos = CCamera::Get()->GetRealPos(vMousePos);

	wchar_t szMousPosLog[255] = {};
	if (KEY_TAP(KEY::RBTN))
	{
		swprintf_s(szMousPosLog, 255, L"TAP X : %d ,Y : %d ", (int)RealMousePos.x, (int)RealMousePos.y);
		MOUSELOG(LOG_LEVEL::WARNING, szMousPosLog);
	}


}
