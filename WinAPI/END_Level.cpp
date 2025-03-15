
#include "END_Level.h"
#include "pch.h"
#include "CEngine.h"
#include "END_Level.h"

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
#include "CBackground.h"


END_Level::END_Level()
{
}

END_Level::~END_Level()
{
}

void END_Level::Init()
{
	Vec2 vResol = CEngine::Get()->GetResolution(); // ������ �ػ� �޾ƿ���

	

	CCamera::Get()->SetFilterEffect(FADE_IN, 3.f);
	CCamera::Get()->SetFilterEffect(FADE_OUT, 3.f);

	CSound* m_Sound = CAssetMgr::Get()->LoadSound(L"Adventurer_Rockstar", L"sound\\Adventurer_Rockstar.wav");
	m_Sound->PlayToBGM(true);

	CBackground* pBack1 = new CBackground;
	//pBack1->SetPos(CCamera::Get()->GetLookAt()+ Vec2(750, 200));
	pBack1->SetPos(CCamera::Get()->GetLookAt());
	pBack1->SetTexture(L"End", L"texture\\End.png");
	AddObject(pBack1, LAYER_TYPE::BACKGROUND);

	CCamera::Get()->SetLookAt(pBack1->GetPos());

	CBackground* pBack2 = new CBackground;
	pBack2->SetPos(pBack1->GetPos() + Vec2(0, 100));
	pBack2->SetTexture(L"Title_Logo_White2", L"texture\\Title_Logo_White.png");
	AddObject(pBack2, LAYER_TYPE::BACKGROUND);

	

}

void END_Level::Exit()
{
	// ��� ������Ʈ�� �����صд�.
	DeleteAllObjects();
}

void END_Level::Tick()
{
	//�ʼ�
	CLevel::Tick();

	//CCamera::Get()->SetLookAt(Vec2(400.f, 400.f));
}