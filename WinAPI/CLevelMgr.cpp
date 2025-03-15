#include "pch.h"
#include "CLevelMgr.h"

#include "CEngine.h"

#include "CLevel.h"
#include "CLevel_Start.h"
#include "CLevel_Stage01.h"
#include "CLevel_Stage02.h"
#include "CLevel_Stage03.h"
#include "CLevel_Stage04.h"
#include "CLevel_Stage05.h"
#include "CLevel_Editor.h"
#include "END_Level.h"

#include "CPlayer.h"
#include "CMonster.h"

#include "CCollisionMgr.h"


CLevelMgr::CLevelMgr()
	: m_arrLevel{}
	, m_CurLevel(nullptr)
{}

CLevelMgr::~CLevelMgr()
{
	for (int i = 0; i < (int)LEVEL_TYPE::END; ++i)
	{
		if (nullptr != m_arrLevel[i])
			delete m_arrLevel[i];
	}
}

void CLevelMgr::Init()
{
	// Level 생성
	m_arrLevel[(int)LEVEL_TYPE::EIDTOR] = new CLevel_Editor;
	m_arrLevel[(int)LEVEL_TYPE::START] = new CLevel_Start;
	m_arrLevel[(int)LEVEL_TYPE::STAGE_01] = new CLevel_Stage01;
	m_arrLevel[(int)LEVEL_TYPE::STAGE_02] = new CLevel_Stage02;
	m_arrLevel[(int)LEVEL_TYPE::STAGE_03] = new CLevel_Stage03;
	m_arrLevel[(int)LEVEL_TYPE::STAGE_04] = new CLevel_Stage04;
	m_arrLevel[(int)LEVEL_TYPE::STAGE_05] = new CLevel_Stage05;
	m_arrLevel[(int)LEVEL_TYPE::END_Level] = new END_Level;

	//현재 레벨 지정
	//STAGE_01
	m_CurLevel = m_arrLevel[(int)LEVEL_TYPE::START];
	m_CurLevel->Init();

}

void CLevelMgr::Tick()
{
	m_CurLevel->Tick();
	m_CurLevel->FinalTick();
}

void CLevelMgr::Render()
{
	m_CurLevel->Render();
}

void CLevelMgr::ChangeLevel(LEVEL_TYPE _Type)
{
	// 변경 전의 레벨을 Exit
	if (nullptr != m_CurLevel)
	{
		m_CurLevel->Exit();
	}

	// 새로운 레벨을 가리키고, 초기화(Init) 을 해둔다.
	m_CurLevel = m_arrLevel[(int)_Type];
	m_CurLevel->Init();
}