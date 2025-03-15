#include "pch.h"
#include "CLevel_Editor.h"
#include "resource.h"

#include "CTile.h"
#include "CPlayer.h"

#include "CLevelMgr.h"
#include "CLevel.h"
#include "CPathMgr.h"
#include "CUI.h"
#include "CBtnUI.h"
#include "CUIMgr.h"

#include "CEditTextObj.h"
#include "CEditAniObj.h"
#include "CPlatform.h"
#include "Cfloor.h"
#include "Monster_Wall.h"

#include "CAssetMgr.h"

#include "CEngine.h"
#include "CTexture.h"
#include "CTileMap.h"
#include "CAnimator.h"
#include "CAnim.h"

#include "CObj.h"
#include "CKeyMgr.h"
#include "CDebugMgr.h"
#include "CLogMgr.h"


#include "CBtnUI.h"
#include "CPanel.h"

#include <windows.h>
#include <string>
#include <tchar.h>

#include "CSound.h"


// �׳� ���漱��
LRESULT CALLBACK TileProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

// �Լ� ������ ��� ����â
void TestFunc()
{
	DialogBox(nullptr, MAKEINTRESOURCE(IDD_TILE_ROWCOL), CEngine::Get()->GetMainHwnd(), TileProc);
	CUIMgr::Get()->SetPreventFrame(1);
}
void TestFunc2()
{
	ChangeLevel(LEVEL_TYPE::START);
}




CLevel_Editor::CLevel_Editor()
	: m_hMenu(nullptr)
	, hDialog(nullptr)
	, m_TileMap(nullptr)
	, m_Animator(nullptr)
	, m_Texture(nullptr)
	, m_EditObj(nullptr)
	, m_AniObj(nullptr)
	, FileName(0)
	, vMousePos(0.f,0.f)
	, RealMousePos(0.f ,0.f)
	, vMouseTapPos(0.f ,0.f)
	, vMouseRealTapPos(0.f ,0.f)
	, vMouseRealReleasedPos(0.f ,0.f)
	, m_vecEditMap()
	, QON(false)
	, EON(false)
{
	m_hMenu = LoadMenu(nullptr, MAKEINTRESOURCE(IDC_WINAPI));
}

CLevel_Editor::~CLevel_Editor()
{
	DestroyMenu(m_hMenu);
}



void CLevel_Editor::Init()
{
	// �޴� �߰�	
	SetMenu(CEngine::Get()->GetMainHwnd(), m_hMenu);



	// �޴��� �پ����ϱ�, �ٽ� ������ ũ�� ����
	UINT width = CEngine::Get()->GetResolution().x;
	UINT height = CEngine::Get()->GetResolution().y;
	CEngine::Get()->ChangeWindowResolution((float)width, (float)height);

	CCamera::Get()->SetLookAt(Vec2( (float)width /2, (float)height/2));

	

	// ������Ʈ ����
	CObj* pObj = nullptr;

	 //Tile
	pObj = new CTile;
	pObj->SetName(L"Tile Object");
	pObj->SetPos(0.f, 0.f);
	m_TileMap = pObj->GetComponent<CTileMap>();
	m_TileMap->SetRowCol(0, 0);
	AddObject(pObj, LAYER_TYPE::TILE);

	// ���� ������Ʈ ����
	m_EditObj = new CEditTextObj;
	m_EditObj->SetPos(Vec2(0.f, 0.f));
	AddObject(m_EditObj, LAYER_TYPE::PLAYER);

	m_AniObj = new CEditAniObj;
	m_AniObj->SetPos(Vec2(700.f, 400.f));
	AddObject(m_AniObj, LAYER_TYPE::PLAYER);

	//CPlayer* player = new CPlayer;
	//player->SetName(L"Player");
	//player->SetPos(Vec2(500.f, 500.f));
	//
	//m_Animator = player->GetComponent<CAnimator>();
	//AddObject(player, LAYER_TYPE::PLAYER);
	

	// Panel �߰�
	//CPanel* pPanel = new CPanel;
	//pPanel->SetScale(Vec2(500.f, 400.f));
	//pPanel->SetPos(Vec2(700.f, 30.f));
	//AddObject(pPanel, LAYER_TYPE::UI);


	// UI �߰��ϱ�
	//CBtnUI* pUI = new CBtnUI;
	//Vec2 vResol = CEngine::Get()->GetResolution();
	//pUI->SetScale(Vec2(120.f, 50.f));
	//pUI->SetPos(Vec2(10.f, 20.f));
	//pUI->SetCallBack(&TestFunc);
	//pPanel->AddChildUI(pUI);
	//
	//pUI = new CBtnUI;
	//pUI->SetScale(Vec2(120.f, 50.f));
	//pUI->SetPos(Vec2(10.f, 120.f));
	//pUI->SetCallBack(&TestFunc2);
	//pPanel->AddChildUI(pUI);
	//
	//pUI = new CBtnUI;
	//pUI->SetScale(Vec2(120.f, 50.f));
	//pUI->SetPos(Vec2(10.f, 220.f));
	//pUI->SetDelegate(this, (BaseFunc)&CLevel_Editor::SaveTile);
	//pPanel->AddChildUI(pUI);
	//
	//pUI = new CBtnUI;
	//pUI->SetScale(Vec2(120.f, 50.f));
	//pUI->SetPos(Vec2(10.f, 320.f));
	//pUI->SetDelegate(this, (BaseFunc)&CLevel_Editor::LoadTile);
	//pPanel->AddChildUI(pUI);

}

void CLevel_Editor::Exit()
{
	// �޴� ����
	SetMenu(CEngine::Get()->GetMainHwnd(), nullptr);

	// �޴��� ���ŵǾ�����, ������ ũ�� �ٽ� ���
	UINT width = CEngine::Get()->GetResolution().x;
	UINT height = CEngine::Get()->GetResolution().y;
	CEngine::Get()->ChangeWindowResolution(width, height);



	// ������Ʈ ����
	DeleteAllObjects();
}

void CLevel_Editor::Tick()
{
	CLevel::Tick();

	if (KEY_TAP(KEY::Q))
	{
		QON = QON ? false : true;
	}

	if (KEY_TAP(KEY::E))
	{
		EON = EON ? false : true;
	}

	// �ִϸ��̼� ���� ���콺üũ
	AniMouseMode();
	
	
	// Ÿ�� ���� ���콺 üũ
	TileMode();

}


void CLevel_Editor::AniMouseMode()
{
	
	vMousePos = CKeyMgr::Get()->GetMousePos();
	RealMousePos = CCamera::Get()->GetRealPos(vMousePos);

	if (KEY_TAP(KEY::ENTER))
	{
		ChangeLevel(LEVEL_TYPE::START);
		CCamera::Get()->SetFilterEffect(FADE_OUT, 0.5f);
		CCamera::Get()->SetFilterEffect(FADE_IN, 0.5f);
	}

	wchar_t szMousPosLog[255] = {};
	if (KEY_TAP(KEY::RBTN))
	{
		swprintf_s(szMousPosLog, 255, L"TAP X : %d ,Y : %d ", (int)RealMousePos.x, (int)RealMousePos.y);
		MOUSELOG(LOG_LEVEL::WARNING, szMousPosLog);
		vMouseTapPos = vMousePos;
		vMouseRealTapPos = RealMousePos;
	}

	if (KEY_PRESSED(KEY::RBTN))
	{
		Vec2 vDiff = vMousePos - vMouseTapPos;
		DrawDebugShape(DEBUG_SHAPE::MOUSE, PEN_TYPE::GREEN, BRUSH_TYPE::HOLLOW, vMouseTapPos, vDiff);
	}
	if (KEY_RELEASED(KEY::RBTN))
	{
		swprintf_s(szMousPosLog, 255, L"RELEASED X : %d , Y : %d ", (int)RealMousePos.x, (int)RealMousePos.y);
		MOUSELOG(LOG_LEVEL::WARNING, szMousPosLog);
		vMouseReleasedPos = vMousePos;
		vMouseRealReleasedPos = RealMousePos;

		// AnProc�� �޽��� ������
		SendMessage(hDialog, WM_COMMAND, MAKEWPARAM(IDC_BTEST, 0), 0);
	}

}


void CLevel_Editor::TileMode()
{
	Vec2 vvMousePos = CKeyMgr::Get()->GetMousePos();
	vvMousePos = CCamera::Get()->GetRealPos(vvMousePos);

	// 1. �� ���콺 ��ǥ�� ���ؼ� �� ��ġ�� ������ �ν��ϸ�
	// 2. Ÿ�� ������� ������ �� �� �� �������� �����ϱ� �� ���� �̿��ؼ�
	// 3. GetTileInfo�� �־ �� ���콺�� �ִ� Ÿ���� ������ ������

	Vec2 TileSize = m_TileMap->GetTileSize();

	UINT Col = vvMousePos.x / TileSize.x;
	UINT Row = vvMousePos.y / TileSize.y;

	tTileInfo* pTileInfo = m_TileMap->GetTileInfo(Col, Row);

	if (nullptr != pTileInfo)
	{
		if (KEY_PRESSED(KEY::RBTN))
		{
			pTileInfo->ImgIdx = 30;
		}

		if (!QON && KEY_PRESSED(KEY::VK_1))
		{
			pTileInfo->ImgIdx = 0;
		}
		else if (QON && KEY_PRESSED(KEY::VK_1))
		{
			pTileInfo->ImgIdx = 8;
		}
		
		if (!QON && KEY_PRESSED(KEY::VK_2))
		{
			pTileInfo->ImgIdx = 1;
		}
		else if (QON && KEY_PRESSED(KEY::VK_2))
		{
			pTileInfo->ImgIdx = 9;
		}
		
		if (!QON && KEY_PRESSED(KEY::VK_3))
		{
			pTileInfo->ImgIdx = 2;
		}
		else if (QON && KEY_PRESSED(KEY::VK_3))
		{
			pTileInfo->ImgIdx = 10;
		}
		
		if (!QON && KEY_PRESSED(KEY::VK_4))
		{
			pTileInfo->ImgIdx = 3;
		}
		else if (QON && KEY_PRESSED(KEY::VK_4))
		{
			pTileInfo->ImgIdx = 11;
		}
		
		if (!QON && KEY_PRESSED(KEY::VK_5))
		{
			pTileInfo->ImgIdx = 4;
		}
		else if (QON && KEY_PRESSED(KEY::VK_5))
		{
			pTileInfo->ImgIdx = 12;
		}
		
		if (!QON && KEY_PRESSED(KEY::VK_6))
		{
			pTileInfo->ImgIdx = 5;
		}
		else if (QON && KEY_PRESSED(KEY::VK_6))
		{
			pTileInfo->ImgIdx = 13;
		}
		
		if (!QON && KEY_PRESSED(KEY::VK_7))
		{
			pTileInfo->ImgIdx = 6;
		}
		else if (QON && KEY_PRESSED(KEY::VK_7))
		{
			pTileInfo->ImgIdx = 14;
		}
		
		if (!QON && KEY_PRESSED(KEY::VK_8))
		{
			pTileInfo->ImgIdx = 7;
		}
		else if (QON && KEY_PRESSED(KEY::VK_8))
		{
			pTileInfo->ImgIdx = 15;
		}
	}


	//���콺 Ŭ������ �÷��� �� �÷ξ� �����
	if (KEY_PRESSED(KEY::F))
	{
		wchar_t szMousPosLog[255] = {};


		if (KEY_TAP(KEY::LBTN))
		{
			swprintf_s(szMousPosLog, 255, L"TAP X : %d ,Y : %d ", (int)RealMousePos.x, (int)RealMousePos.y);
			MOUSELOG(LOG_LEVEL::WARNING, szMousPosLog);
			vMouseTapPos = vMousePos;
			vMouseRealTapPos = RealMousePos;

		}

		if (KEY_PRESSED(KEY::LBTN))
		{
			// Vec2 vDiff = vMousePos - vMouseTapPos;
			//DrawDebugShape(DEBUG_SHAPE::MOUSE, PEN_TYPE::GREEN, BRUSH_TYPE::HOLLOW, vMouseTapPos, vDiff);

			Vec2 vDiff = vvMousePos - vMouseRealTapPos;
			DrawDebugShape(DEBUG_SHAPE::MOUSE, PEN_TYPE::GREEN, BRUSH_TYPE::HOLLOW, vMouseRealTapPos, vDiff);
		}
		if (KEY_RELEASED(KEY::LBTN))
		{
			swprintf_s(szMousPosLog, 255, L"RELEASED X : %d , Y : %d ", (int)RealMousePos.x, (int)RealMousePos.y);
			MOUSELOG(LOG_LEVEL::WARNING, szMousPosLog);
			vMouseReleasedPos = vMousePos;
			vMouseRealReleasedPos = RealMousePos;

			Vec2 vDiff = RealMousePos - vMouseRealTapPos;

			CPlatform* pfloor = new CPlatform;
			pfloor->SetName(L"Platform");
			pfloor->SetPos(vMouseRealTapPos + vDiff / 2);
			pfloor->SetScale(vDiff);
			AddObject(pfloor, LAYER_TYPE::PLATFORM);

			// ���Ϳ� ��������
			tEditMapInfo MapInfo{};
			MapInfo.Type = 1;
			MapInfo.StartPos = (vMouseRealTapPos + vDiff / 2);
			MapInfo.Scale = (vDiff);

			m_vecEditMap.push_back(MapInfo);
		}
	}

	if (KEY_PRESSED(KEY::G))
	{
		wchar_t szMousPosLog[255] = {};

		if (KEY_TAP(KEY::LBTN))
		{
			swprintf_s(szMousPosLog, 255, L"TAP X : %d ,Y : %d ", (int)RealMousePos.x, (int)RealMousePos.y);
			MOUSELOG(LOG_LEVEL::WARNING, szMousPosLog);
			vMouseTapPos = vMousePos;
			vMouseRealTapPos = RealMousePos;
		}

		if (KEY_PRESSED(KEY::LBTN))
		{
			Vec2 vDiff = vMousePos - vMouseTapPos;
			DrawDebugShape(DEBUG_SHAPE::MOUSE, PEN_TYPE::GREEN, BRUSH_TYPE::HOLLOW, vMouseTapPos, vDiff);
		}
		if (KEY_RELEASED(KEY::LBTN))
		{
			swprintf_s(szMousPosLog, 255, L"RELEASED X : %d , Y : %d ", (int)RealMousePos.x, (int)RealMousePos.y);
			MOUSELOG(LOG_LEVEL::WARNING, szMousPosLog);
			vMouseReleasedPos = vMousePos;
			vMouseRealReleasedPos = RealMousePos;

			Vec2 vDiff = RealMousePos - vMouseRealTapPos;

			Cfloor* pfloor = new Cfloor;
			pfloor->SetName(L"Floor");
			pfloor->SetPos(vMouseRealTapPos + vDiff / 2);
			pfloor->SetScale(vDiff);
			AddObject(pfloor, LAYER_TYPE::FLOOR);

			// ���Ϳ� ��������
			tEditMapInfo MapInfo{};
			MapInfo.Type = 2;
			MapInfo.StartPos = (vMouseRealTapPos + vDiff / 2);
			MapInfo.Scale = (vDiff);

			m_vecEditMap.push_back(MapInfo);
		}
	}

	if (KEY_PRESSED(KEY::H))
	{
		wchar_t szMousPosLog[255] = {};

		if (KEY_TAP(KEY::LBTN))
		{
			swprintf_s(szMousPosLog, 255, L"TAP X : %d ,Y : %d ", (int)RealMousePos.x, (int)RealMousePos.y);
			MOUSELOG(LOG_LEVEL::WARNING, szMousPosLog);
			vMouseTapPos = vMousePos;
			vMouseRealTapPos = RealMousePos;
		}

		if (KEY_PRESSED(KEY::LBTN))
		{
			Vec2 vDiff = vMousePos - vMouseTapPos;
			DrawDebugShape(DEBUG_SHAPE::MOUSE, PEN_TYPE::GREEN, BRUSH_TYPE::HOLLOW, vMouseTapPos, vDiff);
		}
		if (KEY_RELEASED(KEY::LBTN))
		{
			swprintf_s(szMousPosLog, 255, L"RELEASED X : %d , Y : %d ", (int)RealMousePos.x, (int)RealMousePos.y);
			MOUSELOG(LOG_LEVEL::WARNING, szMousPosLog);
			vMouseReleasedPos = vMousePos;
			vMouseRealReleasedPos = RealMousePos;

			Vec2 vDiff = RealMousePos - vMouseRealTapPos;

			Monster_Wall* Wall = new Monster_Wall;
			Wall->SetName(L"Wall");
			Wall->SetPos(vMouseRealTapPos + vDiff / 2);
			Wall->SetScale(vDiff);
			AddObject(Wall, LAYER_TYPE::FLOOR);

			// ���Ϳ� ��������
			tEditMapInfo MapInfo{};
			MapInfo.Type = 3;
			MapInfo.StartPos = (vMouseRealTapPos + vDiff / 2);
			MapInfo.Scale = (vDiff);

			m_vecEditMap.push_back(MapInfo);
		}
	}
}

//void CLevel_Editor::TileMode()
//{
//	static Vec2 vvMouseStartPos;
//	static bool bDragging = false;
//
//	Vec2 vvMousePos = CKeyMgr::Get()->GetMousePos();
//	vvMousePos = CCamera::Get()->GetRealPos(vvMousePos);
//
//	if (KEY_TAP(KEY::LBTN))
//	{
//		// ���콺 ��ư�� ������ ���� ��ġ�� �����ϰ� �巡�� ����
//		vvMouseStartPos = vvMousePos;
//		bDragging = true;
//	}
//
//	if (KEY_PRESSED(KEY::LBTN))
//	{
//		if (bDragging)
//		{
//			Vec2 TileSize = m_TileMap->GetTileSize();
//
//			// �巡���� ������ �ּ�/�ִ� ��ǥ�� ���մϴ�.
//			float fMinX = min(vvMouseStartPos.x, vvMousePos.x);
//			float fMaxX = max(vvMouseStartPos.x, vvMousePos.x);
//			float fMinY = min(vvMouseStartPos.y, vvMousePos.y);
//			float fMaxY = max(vvMouseStartPos.y, vvMousePos.y);
//
//			// ����/�� ��ǥ�� Ÿ�� �ε����� ��ȯ�մϴ�.
//			UINT StartCol = fMinX / TileSize.x;
//			UINT EndCol = fMaxX / TileSize.x;
//			UINT StartRow = fMinY / TileSize.y;
//			UINT EndRow = fMaxY / TileSize.y;
//
//			// �巡���� ������ ��� Ÿ���� ������Ʈ�մϴ�.
//			for (UINT Row = StartRow; Row <= EndRow; ++Row)
//			{
//				for (UINT Col = StartCol; Col <= EndCol; ++Col)
//				{
//					tTileInfo* pTileInfo = m_TileMap->GetTileInfo(Col, Row);
//					if (nullptr != pTileInfo)
//					{
//						++pTileInfo->ImgIdx;
//					}
//				}
//			}
//
//			// �巡�� ����
//			bDragging = false;
//		}
//	}
//}

void CLevel_Editor::SaveTile()
{

	// ��� ����ְ�
	wstring StrContentPath = CPathMgr::Get()->GetContentPath();
	StrContentPath += L"tile\\";

	wchar_t szFilePath[255] = {}; // �迭 �ʱ�ȭ

	OPENFILENAME Desc = {}; // ���� ����ü �ʱ�ȭ
	Desc.lStructSize = sizeof(OPENFILENAME); //������
	Desc.hwndOwner = nullptr; // �� ���߿���
	Desc.lpstrFile = szFilePath; // �Ʊ� �ʱ�ȭ�� �迭 �־���
	Desc.nMaxFile = 255; //�츮�� �������� ������ �迭�� �ִ� ����
	// �̰� ���͸� �߰��ϴ°ǵ� Ÿ�� ���Ͽ� ������ * �̸��� ������ٴ� ��
	// ������ Ȯ���ڸ��� .tile �̿����ϰ� ALL ��� ������ �����ϰ� *.* ���� �������
	Desc.lpstrFilter = L"Tile\0*.tile\0ALL\0*.*";
	Desc.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST; // �̰� ����
	Desc.lpstrInitialDir = StrContentPath.c_str();
	// ���Ͽ������� ��θ� ��������

		

	// �����찡 �� ������ ������� ������ �ϳ��� ����� Ž���� â
	// �׸��� �ű⿡ �ɼ��� ��������� �ȴ�.
	// ��ȯ������ Ȯ���� �������� , ��Ҹ� �������� �� Ȯ���Ҽ��� �ִ�.
	// ���� �ٸ��͵� �����ϰ��� �׷�
	// �� �Լ��� ����� �츮�� ������ ���� ��θ��� �˷��� ���̴�.
	if (GetSaveFileName(&Desc))
	{
		if (wcslen(szFilePath) != 0)    // ������ ��ΰ� ���� �ø� ���̺긦 �����ϰ� ����ó��. (������ ����������)
		{
			m_TileMap->Save(szFilePath);
		}
	}

	CUIMgr::Get()->SetPreventFrame(1);
}

void CLevel_Editor::LoadTile()
{

	// ��� ����ְ�

	wstring StrContentPath = CPathMgr::Get()->GetContentPath();
	StrContentPath += L"tile\\";

	wchar_t szFilePath[255] = {}; // �迭 �ʱ�ȭ

	OPENFILENAME Desc = {}; // ���� ����ü �ʱ�ȭ
	Desc.lStructSize = sizeof(OPENFILENAME); //������
	Desc.hwndOwner = nullptr; // �� ���߿���
	Desc.lpstrFile = szFilePath; // �Ʊ� �ʱ�ȭ�� �迭 �־���
	Desc.nMaxFile = 255; //�츮�� �������� ������ �迭�� �ִ� ����
	// �̰� ���͸� �߰��ϴ°ǵ� Ÿ�� ���Ͽ� ������ * �̸��� ������ٴ� ��
	// ������ Ȯ���ڸ��� .tile �̿����ϰ� ALL ��� ������ �����ϰ� *.* ���� �������
	Desc.lpstrFilter = L"ALL\0*.*";
	Desc.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST; // �̰� ����
	Desc.lpstrInitialDir = StrContentPath.c_str();
	// ���Ͽ������� ��θ� ��������

	// �������� �ٸ��Ŵ� �����ֳ� �����ϳ��� �� �Լ� ���̻�
	if (GetOpenFileName(&Desc))
	{
		if (wcslen(szFilePath) != 0)    // ������ ��ΰ� ���� �ø� �ε带 �����ϰ� ����ó��. (������ ����������)
		{
			m_TileMap->Load(szFilePath);
		}
	}

	CUIMgr::Get()->SetPreventFrame(1);
}



// Ÿ�ϰ��� �޽��� ó���� ���ν���
INT_PTR CALLBACK TileProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK )
		{
			// ��ư � �Է��� ���� ���� �������� �Լ� 
			// ������� ���̵� �˷���
			int Col = GetDlgItemInt(hWnd, IDC_Col, nullptr, true);
			int Row = GetDlgItemInt(hWnd, IDC_Row, nullptr, true);

			if (Col == 0 || Row == 0)
			{
				MessageBox(nullptr,L"���� ����", L"Ÿ���� ��,���� 0�� �ɼ� �����ϴ�", MB_OK);
				return (INT_PTR)TRUE;
			}

			// 1. ���� ������ �����ؾ��Ѵ� GetCurrentLevel�� ���������ͷ� �ִϱ� ������ ������ �ٿ�ĳ������ �ؼ� ����Ѵ�.
			CLevel* pCurLevel = CLevelMgr::Get()->GetCurrentLevel();
			CLevel_Editor* pEditorLevel = dynamic_cast<CLevel_Editor*>(pCurLevel);

			// ���� ������ ������ �ƴѰ����� �޴��ٰ� ȣ�� �ƴٸ� ���� ������  �� ����Ʈ�� ��������״� ���Ʈ�� �Ǵ�.
			assert(pEditorLevel);

			CTileMap* pTileMap = pEditorLevel->GetTileMap();
			pTileMap->SetRowCol(Row, Col);


			EndDialog(hWnd, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		else if (LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hWnd, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}

		break;
	}
	return (INT_PTR)FALSE;
}



extern HINSTANCE g_hInst;
#include <commctrl.h>
#pragma comment(lib, "comctl32.lib")
#include <sstream>



void InitComboBox(HWND hDlg)
{
	HWND hComboBox = GetDlgItem(hDlg, IDC_Savelist);
	SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)L"Player\\");
	SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)L"Player_Attack\\");
	SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)L"Monster\\");
	SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)L"Boss\\");
	SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)L"Effect\\");
	SendMessage(hComboBox, CB_SETCURSEL, 0, 0);  // ù ��° �׸��� �⺻ ����

	// �޺��ڽ��� �� �׸� ���̸� ������
	int itemHeight = SendMessage(hComboBox, CB_GETITEMHEIGHT, 0, 0);

	// �޺��ڽ��� �׸� ���� ������
	int itemCount = SendMessage(hComboBox, CB_GETCOUNT, 0, 0);

	// ��Ӵٿ� ��� ���� ���
	int dropDownHeight = itemHeight * itemCount;

	// �޺��ڽ��� ���̸� ������ �簢���� ����
	RECT rect;
	GetWindowRect(hComboBox, &rect);
	int comboWidth = rect.right - rect.left;

	// ��Ӵٿ� ����� ���̸� �����ϰ� �޺��ڽ��� ������Ʈ
	SetWindowPos(hComboBox, HWND_TOP, 0, 0, comboWidth, dropDownHeight, SWP_NOMOVE | SWP_NOZORDER);
}
void InitSPIN(HWND hWnd, int _spin , int _Edit)
{
	HWND hSpin = GetDlgItem(hWnd, _spin);
	HWND hEdit = GetDlgItem(hWnd, _Edit);

	// ���� ��Ʈ���� ���� ���� (-10000���� 10000����)
	SendMessage(hSpin, UDM_SETRANGE32, -1000, 10000);

	// ���� ��Ʈ�Ѱ� ����Ʈ ��Ʈ�� ����
	SendMessage(hSpin, UDM_SETBUDDY, (WPARAM)hEdit, 0);

	// ���� ��Ʈ�� ��Ÿ�� ���� (UDS_ALIGNRIGHT�� �����Ͽ� ������ ����)
	DWORD spinStyle = (DWORD)GetWindowLong(hSpin, GWL_STYLE);
	spinStyle &= ~UDS_WRAP;  // UDS_WRAP ��Ÿ�� ����
	spinStyle |= UDS_ALIGNRIGHT;  // ������ ���� ��Ÿ�� �߰�
	SetWindowLong(hSpin, GWL_STYLE, spinStyle);

	// ���� ��Ʈ���� ���ǵ�(����) ����
	UDACCEL accel = { 0, 1 };  // {nSec, nInc} - nSec �� �Ŀ� nInc ������ ����/����
	SendMessage(hSpin, UDM_SETACCEL, 1, (LPARAM)&accel);

}


//float ���� wstring�� ��ȯ�ؼ� ����ϴ� �ڵ�
void SetDlgItemFloat(HWND _hWnd, int _nIDDlgItem, float _value)
{
	// float ���� wstring���� ��ȯ
	std::wstringstream wss;		   // wstringstream ��ü ����
	wss << _value;				   // float ���� wstringstream�� ����
	std::wstring wstr = wss.str(); // wstringstream�� wstring���� ��ȯ

	// ��ȯ�� ���ڿ��� ���̾�α� ��Ʈ�ѿ� ����
	SetDlgItemText(_hWnd, _nIDDlgItem, wstr.c_str());
}

// ����Ʈ ��Ʈ�ѿ��� ������ int�� ���������� ���� Ÿ������ ������ float�� ��ȯ�ϴ� �Լ�
float GetDlgItemFloat(HWND _hWnd, int _nIDDlgItem)
{
	// ���̾�α� ��Ʈ�ѿ��� �ؽ�Ʈ�� ��������
	wchar_t buffer[256]; // ����� ū ���� ���
	GetDlgItemText(_hWnd, _nIDDlgItem, buffer, 256);

	// wstring�� float���� ��ȯ
	float floatValue = 0.0f;
	std::wstringstream wss(buffer); // ���۷κ��� wstringstream ��ü ����
	wss >> floatValue;              // wstringstream���� float���� ��ȯ

	return floatValue;
}

struct AniNamePath { std::wstring Name;  std::wstring Path; };

INT_PTR CALLBACK AniProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
	{
		InitComboBox(hWnd);
		InitSPIN(hWnd, IDC_SPIN1, IDC_Left_X);
		InitSPIN(hWnd, IDC_SPIN2, IDC_Left_Y);
		InitSPIN(hWnd, IDC_SPIN3, IDC_OffSet_X);
		InitSPIN(hWnd, IDC_SPIN4, IDC_OffSet_Y);
		InitSPIN(hWnd, IDC_SPIN5, IDC_Slice_X);
		InitSPIN(hWnd, IDC_SPIN6, IDC_Slice_Y);
		InitSPIN(hWnd, IDC_SPIN7, IDC_Scale_X);
		InitSPIN(hWnd, IDC_SPIN8, IDC_Scale_Y);

		SetDlgItemInt(hWnd, IDC_Frm_Count,	0, NULL);
		SetDlgItemFloat(hWnd, IDC_OffSet_X,	0);
		SetDlgItemFloat(hWnd, IDC_OffSet_Y,	0);
		SetDlgItemFloat(hWnd, IDC_Left_X,	0);
		SetDlgItemFloat(hWnd, IDC_Left_Y,	0);
		SetDlgItemFloat(hWnd, IDC_Slice_X,	0);
		SetDlgItemFloat(hWnd, IDC_Slice_Y,	0);
		SetDlgItemFloat(hWnd, IDC_Duration,	0);
	
	}
		return (INT_PTR)TRUE;


	case WM_COMMAND:
		{//���������� �޾ƿ���
		CLevel* pCurLevel = CLevelMgr::Get()->GetCurrentLevel();
		CLevel_Editor* pEditorLevel = dynamic_cast<CLevel_Editor*>(pCurLevel);
		// ���� ������ ������ �ƴѰ����� �޴��ٰ� ȣ�� �ƴٸ� ���� ������  �� ����Ʈ�� ��������״� ���Ʈ�� �Ǵ�.
		assert(pEditorLevel);


		//####################�Է� ��ư ####################
		if (LOWORD(wParam) == IDC_BTEST)
		{

			// ���콺 ��ǥ ���
			Vec2 vMouseTapPos = pEditorLevel->GetRealTapPos();
			Vec2 vMouseReleasedPos = pEditorLevel->GetRealReleasedPos();
			Vec2 SlicePos = vMouseReleasedPos - vMouseTapPos;

			SetDlgItemFloat(hWnd, IDC_Left_X, vMouseTapPos.x);
			SetDlgItemFloat(hWnd, IDC_Left_Y, vMouseTapPos.y);
			SetDlgItemFloat(hWnd, IDC_Slice_X, SlicePos.x);
			SetDlgItemFloat(hWnd, IDC_Slice_Y, SlicePos.y);

			SetDlgItemInt(hWnd, IDC_OffSet_X, 0, true);
			SetDlgItemInt(hWnd, IDC_OffSet_Y, 0, true);
			SetDlgItemInt(hWnd, IDC_Duration, 1, true);

			SendMessage(hWnd, WM_COMMAND, MAKEWPARAM(IDC_T_B, 0), 0);

		}

		//################### �̸����� #################
		else if (LOWORD(wParam) == IDC_T_B)
		{

			float LX = GetDlgItemFloat(hWnd, IDC_Left_X);
			float LY = GetDlgItemFloat(hWnd, IDC_Left_Y);
			float SX = GetDlgItemFloat(hWnd, IDC_Slice_X);
			float SY = GetDlgItemFloat(hWnd, IDC_Slice_Y);
			float OX = GetDlgItemFloat(hWnd, IDC_OffSet_X);
			float OY = GetDlgItemFloat(hWnd, IDC_OffSet_Y);

			// ��ο�
			TCHAR zzBuffer[100];
			GetDlgItemText(hWnd, IDC_Coice, zzBuffer, sizeof(zzBuffer) / sizeof(TCHAR));
			wstring Path = zzBuffer;

			CTexture* texture;

			texture = CAssetMgr::Get()->LoadTexture(Path, Path);

			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);

			// ���� ��Ʈ�� �ڵ� ��������
			HWND hPicture = GetDlgItem(hWnd, IDC_Ani_View);

			// ���� ��Ʈ���� DC ��������
			HDC hdcPicture = GetDC(hPicture);

			// ���� ��Ʈ���� Ŭ���̾�Ʈ ���� ��������
			RECT rect;
			GetClientRect(hPicture, &rect);
			
			// ����� ���� �귯�ø� ����Ͽ� ���� ��Ʈ���� ����� ȸ������ �����
			HBRUSH hBrush = CreateSolidBrush(RGB(100, 100, 100)); // RGB ������ ȸ�� ����
			FillRect(hdcPicture, &rect, hBrush);
			// �귯�� ����
			DeleteObject(hBrush);

			// ���� �巡���� ���̿� ����
			UINT width = (UINT)SX;
			UINT height = (UINT)SY;

			// �߽� ��ǥ ���
			int centerX = rect.right / 2;
			int centerY = rect.bottom / 2;

			// �巡�׵� ũ�⸦ ������ �׸��� ��ġ ��� (�߽� ����)
			int x = centerX - width / 2 + (int)OX;
			int y = centerY - height / 2 + (int)OY;

			BLENDFUNCTION blend = {};
			blend.BlendOp = AC_SRC_OVER;
			blend.BlendFlags = 0;
			blend.SourceConstantAlpha = 255;
			blend.AlphaFormat = AC_SRC_ALPHA;
			AlphaBlend(hdcPicture, x, y, width, height, texture->GetDC(), (int)LX, (int)LY, width, height, blend);

			// ����� �׸����
			Vec2 rectVec2 = Vec2((float)rect.right, (float)rect.bottom);

			DrawDebugShape(DEBUG_SHAPE::CROSS, PEN_TYPE::GREEN, BRUSH_TYPE::HOLLOW, rectVec2, rectVec2);

			// �� ����
			SELECT_PEN(hdcPicture, PEN_TYPE::GREEN);
			// �귯�� ����
			SELECT_BRUSH(hdcPicture, BRUSH_TYPE::HOLLOW);

			float SDX = GetDlgItemFloat(hWnd, IDC_Scale_X);
			float SDY = GetDlgItemFloat(hWnd, IDC_Scale_Y);
			float SDO = GetDlgItemFloat(hWnd, IDC_Scale_O); // �ݶ��̴� �����¿� ���� Y�� ��

			Rectangle(hdcPicture
				, rect.right / 2 - (int)SDX / 2
				, rect.bottom / 2 - (int)SDY / 2 + (int)SDO
				, rect.right / 2 + (int)SDX / 2
				, rect.bottom/2 + (int)SDY/2 + (int)SDO);

			EndPaint(hWnd, &ps);
		}

		//####################�ִ���� ####################
		else if (LOWORD(wParam) == IDC_Play)
		{
			TCHAR azBuffer[100];
			GetDlgItemText(hWnd, IDC_Set_Name, azBuffer, sizeof(azBuffer) / sizeof(TCHAR));
			wstring Nmae = azBuffer;

			pEditorLevel->EditPlay(Nmae);
		}
		//####################�ִ� ����� ####################
		else if (LOWORD(wParam) == IDC_Stop)
		{
			// �̸� ������
			TCHAR azBuffer[100];
			GetDlgItemText(hWnd, IDC_Set_Name, azBuffer, sizeof(azBuffer) / sizeof(TCHAR));
			wstring Nmae = azBuffer;

			CAnimator* Ani = pEditorLevel->GetEditObj()->GetEditAnimator();
			Ani->Stop();
			Ani->DeleteAnimation(Nmae);

		}

		//####################�ִ� ���� ####################
		else if (LOWORD(wParam) == IDC_Ani_Creat)
		{
			float LX = GetDlgItemFloat(hWnd, IDC_Left_X);
			float LY = GetDlgItemFloat(hWnd, IDC_Left_Y);
			float SX = GetDlgItemFloat(hWnd, IDC_Slice_X);
			float SY = GetDlgItemFloat(hWnd, IDC_Slice_Y);
			float OX = GetDlgItemFloat(hWnd, IDC_OffSet_X);
			float OY = GetDlgItemFloat(hWnd, IDC_OffSet_Y);
			float DU = GetDlgItemFloat(hWnd, IDC_Duration);


			// ��ο�
			TCHAR zzBuffer[100];
			GetDlgItemText(hWnd, IDC_Coice, zzBuffer, sizeof(zzBuffer) / sizeof(TCHAR));
			wstring Path = zzBuffer;

			// �̸� ������
			TCHAR azBuffer[100];
			GetDlgItemText(hWnd, IDC_Set_Name, azBuffer, sizeof(azBuffer) / sizeof(TCHAR));
			wstring Nmae = azBuffer;

			pEditorLevel->CreatAni(Nmae, Path, Nmae, 1, 1.f, Vec2(SX, SY), Vec2(LX, LY));

			//���̾�α� �Ȳ�
			return (INT_PTR)TRUE;
		}

		//####################�ؽ������� ���� ####################
		else if (LOWORD(wParam) == IDC_ATLAS_Select) // ��Ʋ�� ����Ʈ ������
		{
			wstring FileName = pEditorLevel->SelectAtlas();


			SetDlgItemText(hWnd, IDC_Coice, FileName.c_str());

			//���̾�α� �Ȳ�
			return (INT_PTR)TRUE;
		}

		//####################����� �ִϸ��̼� �ҷ����� ####################
		else if (LOWORD(wParam) == IDC_ATLAS_LOAD) // ��Ʋ�� �ε� ������
		{

			AniNamePath NamePath = pEditorLevel->LoadAni();

			SetDlgItemText(hWnd, IDC_Atlas_Name, NamePath.Name.c_str());

			SetDlgItemText(hWnd, IDC_Coice, NamePath.Path.c_str());

			//���̾�α� ���� 
			return (INT_PTR)TRUE;
		}

		//#################### ���� �ִϸ��̼� �����ϱ� ####################
		else if (LOWORD(wParam) == IDC_ANI_SAVE) // �ִ� ���̺� ������
		{

			// �޺��ڽ� �ڵ� ��������
			HWND hComboBox = GetDlgItem(hWnd, IDC_Savelist);
			// ���õ� �׸��� �ε����� ��������
			int index = SendMessage(hComboBox, CB_GETCURSEL, 0, 0);
			// ���õ� �׸��� �ؽ�Ʈ�� �������� ���� ����
			wchar_t selectedText[256] = {};
			// ���õ� �׸��� �ؽ�Ʈ ��������
			SendMessage(hComboBox, CB_GETLBTEXT, index, (LPARAM)selectedText);
			// wchar_t �迭�� wstring���� ��ȯ
			wstring selectedString = selectedText;

			// �̸� ������
			TCHAR azBuffer[100];
			GetDlgItemText(hWnd, IDC_Set_Name, azBuffer, sizeof(azBuffer) / sizeof(TCHAR));
			wstring Name = azBuffer;

			//selectedString �ڸ����� � ������ �����Ұ��� Name �� ���� ������ �̸�
			pEditorLevel->SaveAni(selectedString ,Name);
			{
				wstring Logg = Name + L"��" + selectedString + L"�� ���� �Ǿ����ϴ�.";
				MessageBox(nullptr, Logg.c_str(), L"���� �Ϸ�", MB_OK);
				return (INT_PTR)TRUE;

			}


			//���̾�α� ���� 
			return (INT_PTR)TRUE;
		}

		//####################������ �ϳ� �߰��ϱ� ####################
		else if (LOWORD(wParam) == IDC_Frm_Add) // ������ �߰� ������ ���� ���콺 Ŭ���ϸ� �����ϰ� �ؾ���
		{
			// ó���� �����ߴ� ��޸��� â �����
			//HWND hDialog = nullptr;
			//hDialog = CreateDialog(g_hInst, MAKEINTRESOURCE(IDD_FrmAdd), hWnd, FrmProc);
			//ShowWindow(hDialog, SW_SHOW);

			// ���콺 ��ǥ ���
			Vec2 vMouseTapPos = pEditorLevel->GetRealTapPos();
			Vec2 vMouseReleasedPos = pEditorLevel->GetRealReleasedPos();
			Vec2 SlicePos = vMouseReleasedPos - vMouseTapPos;

			SetDlgItemFloat(hWnd, IDC_Left_X, vMouseTapPos.x);
			SetDlgItemFloat(hWnd, IDC_Left_Y, vMouseTapPos.y);
			SetDlgItemFloat(hWnd, IDC_Slice_X, SlicePos.x);
			SetDlgItemFloat(hWnd, IDC_Slice_Y, SlicePos.y);

			float LX = GetDlgItemFloat(hWnd, IDC_Left_X);
			float LY = GetDlgItemFloat(hWnd, IDC_Left_Y);
			float SX = GetDlgItemFloat(hWnd, IDC_Slice_X);
			float SY = GetDlgItemFloat(hWnd, IDC_Slice_Y);
			float OX = GetDlgItemFloat(hWnd, IDC_OffSet_X);
			float OY = GetDlgItemFloat(hWnd, IDC_OffSet_Y);
			float DU = GetDlgItemFloat(hWnd, IDC_Duration);

			// �̸� ������
			TCHAR azBuffer[100];
			GetDlgItemText(hWnd, IDC_Set_Name, azBuffer, sizeof(azBuffer) / sizeof(TCHAR));
			wstring Nmae = azBuffer;


			pEditorLevel->FrmAdd(Vec2(LX, LY), Vec2(OX, OY), Vec2(SX, SY), DU, Nmae);

			return (INT_PTR)TRUE;
		}

		//################# ������ �����ϱ� ###################
		else if (LOWORD(wParam) == IDC_Frm_Edit)
		{
			float LX = GetDlgItemFloat(hWnd, IDC_Left_X);
			float LY = GetDlgItemFloat(hWnd, IDC_Left_Y);
			float SX = GetDlgItemFloat(hWnd, IDC_Slice_X);
			float SY = GetDlgItemFloat(hWnd, IDC_Slice_Y);
			float OX = GetDlgItemFloat(hWnd, IDC_OffSet_X);
			float OY = GetDlgItemFloat(hWnd, IDC_OffSet_Y);
			float DU = GetDlgItemFloat(hWnd, IDC_Duration);

			int idx = GetDlgItemInt(hWnd, IDC_Frm_Count, NULL, false);

			// �̸� ������
			TCHAR azBuffer[100];
			GetDlgItemText(hWnd, IDC_Set_Name, azBuffer, sizeof(azBuffer) / sizeof(TCHAR));
			wstring Nmae = azBuffer;
	
			pEditorLevel->FrmSave(Vec2(LX, LY), Vec2(OX, OY), Vec2(SX, SY), DU, Nmae, idx);

		}

		//################## �෹�̼� �ϰ� ���� ################
		else if (LOWORD(wParam) == IDC_Duration_B)
		{
			float DU = GetDlgItemFloat(hWnd, IDC_Duration);

			// �̸� ������
			TCHAR azBuffer[100];
			GetDlgItemText(hWnd, IDC_Set_Name, azBuffer, sizeof(azBuffer) / sizeof(TCHAR));
			wstring Nmae = azBuffer;

			pEditorLevel->DurationSave(Nmae, DU);

		}

		
		//####################Ȯ�� ��ư ####################
		else if (LOWORD(wParam) == IDOK)
		{
			EndDialog(hWnd, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}

		//####################������ ~ ####################
		else if (LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hWnd, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
	}
		break;

	//###################### ���� ��Ʈ��#################
	case WM_NOTIFY:
		{
		// ������Ʈ�ѿ� ���� ����
		long long wmId = LOWORD(wParam);
		long long wmEvent = HIWORD(wParam);

		long long controlId = LOWORD(wParam);  // ��Ʈ���� ID�� ����ϴ�.
		NMHDR* nmhdr = (NMHDR*)lParam;  // NMHDR ����ü�� ����ϴ�.

		// ���� ��Ʈ�ѿ��� �߻��� �˸����� Ȯ���մϴ�.
		if (controlId == IDC_SPIN1 && nmhdr->code == UDN_DELTAPOS)
		{
			// NMUPDOWN ����ü�κ��� ���� ��Ʈ���� ���� ������ ����ϴ�.
			NMUPDOWN* nmud = (NMUPDOWN*)lParam;

			// ���� ����Ʈ ��Ʈ���� ���� �����ɴϴ�.
			long long currentValue = GetDlgItemFloat(hWnd, IDC_Left_X);

			// ����� ���� ����Ͽ� ����Ʈ ��Ʈ�ѿ� �����մϴ�.
			long long newValue = currentValue + nmud->iDelta;

			SetDlgItemFloat(hWnd, IDC_Left_X, (float)newValue);

			SendMessage(hWnd, WM_COMMAND, MAKEWPARAM(IDC_T_B, 0), 0);
		}

		else if (controlId == IDC_SPIN2 && nmhdr->code == UDN_DELTAPOS)
		{
			// NMUPDOWN ����ü�κ��� ���� ��Ʈ���� ���� ������ ����ϴ�.
			NMUPDOWN* nmud = (NMUPDOWN*)lParam;

			// ���� ����Ʈ ��Ʈ���� ���� �����ɴϴ�.
			long long currentValue = GetDlgItemFloat(hWnd, IDC_Left_Y);

			// ����� ���� ����Ͽ� ����Ʈ ��Ʈ�ѿ� �����մϴ�.
			long long newValue = currentValue + nmud->iDelta;

			SetDlgItemFloat(hWnd, IDC_Left_Y, (float)newValue);

			SendMessage(hWnd, WM_COMMAND, MAKEWPARAM(IDC_T_B, 0), 0);
		}

		else if (controlId == IDC_SPIN3 && nmhdr->code == UDN_DELTAPOS)
		{
			// NMUPDOWN ����ü�κ��� ���� ��Ʈ���� ���� ������ ����ϴ�.
			NMUPDOWN* nmud = (NMUPDOWN*)lParam;

			// ���� ����Ʈ ��Ʈ���� ���� �����ɴϴ�.
			long long currentValue = GetDlgItemFloat(hWnd, IDC_OffSet_X);

			// ����� ���� ����Ͽ� ����Ʈ ��Ʈ�ѿ� �����մϴ�.
			long long newValue = currentValue + nmud->iDelta;

			SetDlgItemFloat(hWnd, IDC_OffSet_X, (float)newValue);

			SendMessage(hWnd, WM_COMMAND, MAKEWPARAM(IDC_T_B, 0), 0);
		}

		else if (controlId == IDC_SPIN4 && nmhdr->code == UDN_DELTAPOS)
		{
			// NMUPDOWN ����ü�κ��� ���� ��Ʈ���� ���� ������ ����ϴ�.
			NMUPDOWN* nmud = (NMUPDOWN*)lParam;

			// ���� ����Ʈ ��Ʈ���� ���� �����ɴϴ�.
			long long currentValue = GetDlgItemFloat(hWnd, IDC_OffSet_Y);

			// ����� ���� ����Ͽ� ����Ʈ ��Ʈ�ѿ� �����մϴ�.
			long long newValue = currentValue + nmud->iDelta;

			SetDlgItemFloat(hWnd, IDC_OffSet_Y, (float)newValue);

			SendMessage(hWnd, WM_COMMAND, MAKEWPARAM(IDC_T_B, 0), 0);
		}

		else if (controlId == IDC_SPIN5 && nmhdr->code == UDN_DELTAPOS)
		{
			// NMUPDOWN ����ü�κ��� ���� ��Ʈ���� ���� ������ ����ϴ�.
			NMUPDOWN* nmud = (NMUPDOWN*)lParam;

			// ���� ����Ʈ ��Ʈ���� ���� �����ɴϴ�.
			long long currentValue = GetDlgItemFloat(hWnd, IDC_Slice_X);

			// ����� ���� ����Ͽ� ����Ʈ ��Ʈ�ѿ� �����մϴ�.
			long long newValue = currentValue + nmud->iDelta;

			SetDlgItemFloat(hWnd, IDC_Slice_X, (float)newValue);

			SendMessage(hWnd, WM_COMMAND, MAKEWPARAM(IDC_T_B, 0), 0);
		}

		else if (controlId == IDC_SPIN6 && nmhdr->code == UDN_DELTAPOS)
		{
			// NMUPDOWN ����ü�κ��� ���� ��Ʈ���� ���� ������ ����ϴ�.
			NMUPDOWN* nmud = (NMUPDOWN*)lParam;

			// ���� ����Ʈ ��Ʈ���� ���� �����ɴϴ�.
			long long currentValue = GetDlgItemFloat(hWnd, IDC_Slice_Y);

			// ����� ���� ����Ͽ� ����Ʈ ��Ʈ�ѿ� �����մϴ�.
			long long newValue = currentValue + nmud->iDelta;

			SetDlgItemFloat(hWnd, IDC_Slice_Y, (float)newValue);

			SendMessage(hWnd, WM_COMMAND, MAKEWPARAM(IDC_T_B, 0), 0);
		}

		else if (controlId == IDC_SPIN7 && nmhdr->code == UDN_DELTAPOS)
		{
			// NMUPDOWN ����ü�κ��� ���� ��Ʈ���� ���� ������ ����ϴ�.
			NMUPDOWN* nmud = (NMUPDOWN*)lParam;

			// ���� ����Ʈ ��Ʈ���� ���� �����ɴϴ�.
			long long currentValue = GetDlgItemFloat(hWnd, IDC_Scale_X);

			// ����� ���� ����Ͽ� ����Ʈ ��Ʈ�ѿ� �����մϴ�.
			long long newValue = currentValue + nmud->iDelta;

			SetDlgItemFloat(hWnd, IDC_Scale_X, (float)newValue);

			SendMessage(hWnd, WM_COMMAND, MAKEWPARAM(IDC_T_B, 0), 0);
		}

		else if (controlId == IDC_SPIN8 && nmhdr->code == UDN_DELTAPOS)
		{
			// NMUPDOWN ����ü�κ��� ���� ��Ʈ���� ���� ������ ����ϴ�.
			NMUPDOWN* nmud = (NMUPDOWN*)lParam;

			// ���� ����Ʈ ��Ʈ���� ���� �����ɴϴ�.
			long long currentValue = GetDlgItemFloat(hWnd, IDC_Scale_Y);

			// ����� ���� ����Ͽ� ����Ʈ ��Ʈ�ѿ� �����մϴ�.
			long long newValue = currentValue + nmud->iDelta;

			SetDlgItemFloat(hWnd, IDC_Scale_Y, (float)newValue);

			SendMessage(hWnd, WM_COMMAND, MAKEWPARAM(IDC_T_B, 0), 0);
		}

		else if (controlId == IDC_Frm_Idx && nmhdr->code == UDN_DELTAPOS)
		{
			HWND hSpin = GetDlgItem(hWnd, IDC_Frm_Idx);
			HWND hEdit = GetDlgItem(hWnd, IDC_Frm_Count);

			TCHAR azBuffer[100];
			GetDlgItemText(hWnd, IDC_Set_Name, azBuffer, sizeof(azBuffer) / sizeof(TCHAR));
			wstring Nmae = azBuffer;

			CLevel* pCurLevel = CLevelMgr::Get()->GetCurrentLevel();
			CLevel_Editor* pEditorLevel = dynamic_cast<CLevel_Editor*>(pCurLevel);

			
			CAnimator* Ani = pEditorLevel->GetEditObj()->GetEditAnimator();
			map<wstring, CAnim*> AniMap = Ani->GetMapAnimator();

			int IDX = 0;

			auto iter = AniMap.find(Nmae);

			if (iter != AniMap.end())
			{
				IDX = iter->second->m_vecFrm.size() - 1; // �ش� ��ü�� ���� ũ�� ��ȯ (0-based index)
			}
			else
			{
				return 0; // Ű�� ã�� ���� ��� 0 ��ȯ (�Ǵ� ���� ó��)
			}

			// ���� ��Ʈ���� ���� ����
			SendMessage(hSpin, UDM_SETRANGE32, 0, IDX);

			// ���� ��Ʈ���� �ʱⰪ ����
			SendMessage(hSpin, UDM_SETPOS32, 0, 0);

			// ���� ��Ʈ�Ѱ� ����Ʈ ��Ʈ�� ����
			SendMessage(hSpin, UDM_SETBUDDY, (WPARAM)hEdit, 0);

			// ���� ��Ʈ�� ��Ÿ�� ���� (UDS_WRAP�� �����Ͽ� ������ �ʰ��ϸ� ó�� ������ �ǵ��ư��� ��)
			DWORD spinStyle = (DWORD)GetWindowLong(hSpin, GWL_STYLE);
			spinStyle |= UDS_WRAP;  // UDS_WRAP ��Ÿ�� �߰�
			spinStyle |= UDS_ALIGNRIGHT;  // ������ ���� ��Ÿ�� �߰�
			SetWindowLong(hSpin, GWL_STYLE, spinStyle);

			// ���� ��Ʈ���� ���ǵ�(����) ����
			UDACCEL accel = { 0, 1 };  // {nSec, nInc} - nSec �� �Ŀ� nInc ������ ����/����
			SendMessage(hSpin, UDM_SETACCEL, 1, (LPARAM)&accel);

			// NMUPDOWN ����ü�κ��� ���� ��Ʈ���� ���� ������ ����ϴ�.
			NMUPDOWN* nmud = (NMUPDOWN*)lParam;

			// ���� ����Ʈ ��Ʈ���� ���� �����ɴϴ�.
			BOOL good;
			int currentValue = GetDlgItemInt(hWnd, IDC_Frm_Count, &good, TRUE);

			// ����� ���� ����մϴ�.
			int newValue = currentValue + nmud->iDelta;

			// IDX ���� ������ �����ϱ� ���� ������ ������ ����մϴ�.
			if (newValue < 0)
				newValue = IDX;
			else if (newValue > IDX)
				newValue = 0;

			// ����Ʈ ��Ʈ�ѿ� �����մϴ�.
			SetDlgItemInt(hWnd, IDC_Frm_Count, newValue, TRUE);


			// �Ʒ� ���ʹ� ������ ���� �о�ͼ� ĭ�� ä���
			int CurIdx = GetDlgItemInt(hWnd, IDC_Frm_Count, NULL, TRUE);

			if (Ani->FindAnimation(Nmae))
			{
				CAnim* FrmIdx = Ani->FindAnimation(Nmae);

				Vec2 Offset = FrmIdx->GetFrame(CurIdx).vOffset;
				Vec2 Left = FrmIdx->GetFrame(CurIdx).vLeftTop;
				Vec2 Slice = FrmIdx->GetFrame(CurIdx).vSlice;
				float Durat = FrmIdx->GetFrame(CurIdx).Duration;

				SetDlgItemFloat(hWnd, IDC_OffSet_X, Offset.x);
				SetDlgItemFloat(hWnd, IDC_OffSet_Y, Offset.y);
				SetDlgItemFloat(hWnd, IDC_Left_X, Left.x);
				SetDlgItemFloat(hWnd, IDC_Left_Y, Left.y);
				SetDlgItemFloat(hWnd, IDC_Slice_X, Slice.x);
				SetDlgItemFloat(hWnd, IDC_Slice_Y, Slice.y);
				SetDlgItemFloat(hWnd, IDC_Duration, Durat);

			}
		
			SendMessage(hWnd, WM_COMMAND, MAKEWPARAM(IDC_T_B, 0), 0);
		}
	}
		return (INT_PTR)TRUE;
		
	}
		
	
	return (INT_PTR)FALSE;
}


wstring CLevel_Editor::CreatAni(const wstring& _Key, const wstring& _Path, const wstring& _Nmae , int _FPS, float _Time, Vec2 _Slice, Vec2 _Left)
{
	tAnimDesc desc = {};
	desc.pAtlas = CAssetMgr::Get()->LoadTexture(_Key, _Path);
	desc.AnimName = _Nmae;
	desc.FPS = _FPS;
	desc.FrmCount = (int)_Time;
	desc.SliceSize = _Slice;
	desc.StartLeftTop = _Left;

	CAnimator* Ani = m_AniObj->GetEditAnimator();
	Ani->CreateAnimation(desc);
	Ani->Play(_Nmae, true);

	return _Nmae;
}

wstring  CLevel_Editor::SelectAtlas()
{
	// ��� ����ְ�
	wstring StrContentPath = CPathMgr::Get()->GetContentPath();
	//StrContentPath += L"texture\\";

	wchar_t szFilePath[255] = {}; // �迭 �ʱ�ȭ

	OPENFILENAME Desc = {}; // ���� ����ü �ʱ�ȭ
	Desc.lStructSize = sizeof(OPENFILENAME); //������
	Desc.hwndOwner = nullptr; // �� ���߿���
	Desc.lpstrFile = szFilePath; // �Ʊ� �ʱ�ȭ�� �迭 �־���
	Desc.nMaxFile = 255; //�츮�� �������� ������ �迭�� �ִ� ����
	// �̰� ���͸� �߰��ϴ°ǵ� Ÿ�� ���Ͽ� ������ * �̸��� ������ٴ� ��
	// ������ Ȯ���ڸ��� .tile �̿����ϰ� ALL ��� ������ �����ϰ� *.* ���� �������
	Desc.lpstrFilter = L"PNG\0*.png\0ALL\0*.*";
	Desc.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST; // �̰� ����
	Desc.lpstrInitialDir = StrContentPath.c_str();
	// ���Ͽ������� ��θ� ��������

	// �����찡 �� ������ ������� ������ �ϳ��� ����� Ž���� â
	// �׸��� �ű⿡ �ɼ��� ��������� �ȴ�.
	// ��ȯ������ Ȯ���� �������� , ��Ҹ� �������� �� Ȯ���Ҽ��� �ִ�.
	// ���� �ٸ��͵� �����ϰ��� �׷�
	// �� �Լ��� ����� �츮�� ������ ���� ��θ��� �˷��� ���̴�.
	if (GetSaveFileName(&Desc))
	{
		if (wcslen(szFilePath) != 0)
		{
			// �� ���� ����
			wstring fullPath = szFilePath;

			// ã���� �ϴ� ��θ� ������ �������� ���� strContentPath ���� �ؽ���\\ ������ ��ΰ� �������
			size_t pos = fullPath.find(StrContentPath);

			if (pos != wstring::npos)
			{
				// 'StrContentPath' ������ ��θ� �����
				fullPath = fullPath.substr(pos + StrContentPath.length());

				// ���� �ؽ��� �ҷ�����
				m_EditObj->SetTexture(CAssetMgr::Get()->LoadEditTexture(fullPath,szFilePath));


				// �޾Ƴ� �̸� ��ȯ���ֱ�
				return fullPath;
			}

		}
	}

	// �Լ��� ��ȯ Ÿ���� wstring �̹Ƿ� ��� ���ǿ� �ɸ��� ���� ��� �⺻���� �� ���ڿ� ��ȯ
	return L"";
}

AniNamePath CLevel_Editor::LoadAni()
{
	// ��ȯ���� ����ü ����
	AniNamePath result = {};

	// ��� ����ְ�
	wstring StrContentPath = CPathMgr::Get()->GetContentPath();
	StrContentPath += L"animation\\";
	wchar_t szFilePath[255] = {}; // �迭 �ʱ�ȭ

	OPENFILENAME Desc = {}; // ���� ����ü �ʱ�ȭ
	Desc.lStructSize = sizeof(OPENFILENAME); //������
	Desc.hwndOwner = nullptr; // �� ���߿���
	Desc.lpstrFile = szFilePath; // �Ʊ� �ʱ�ȭ�� �迭 �־���
	Desc.nMaxFile = 255; //�츮�� �������� ������ �迭�� �ִ� ����

	// �̰� ���͸� �߰��ϴ°ǵ� Ÿ�� ���Ͽ� ������ * �̸��� ������ٴ� ��
	// ������ Ȯ���ڸ��� .tile �̿����ϰ� ALL ��� ������ �����ϰ� *.* ���� �������
	Desc.lpstrFilter = L"animation\0*.anim\0ALL\0*.*";
	Desc.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST; // �̰� ����
	Desc.lpstrInitialDir = StrContentPath.c_str(); // ���Ͽ������� ��θ� ��������
	

	// �������� �ٸ��Ŵ� �����ֳ� �����ϳ��� �� �Լ� ���̻�
	if (GetOpenFileName(&Desc))
	{
		if (wcslen(szFilePath) != 0)
		{
			// �� ���� ����
			wstring fullPath = szFilePath;

			// ��Ʋ�� ���� �ּ� �˾Ƴ��� load �Լ� ����
			FILE* pFile = nullptr;
			_wfopen_s(&pFile, fullPath.c_str(), L"r");

			wchar_t szRead[255] = {};

			if (!pFile)
			{
				// ���� ���⿡ ������ ���
				MessageBox(NULL, fullPath.c_str(), L"Error", MB_ICONERROR);
				return result; // �Լ� ����
			}

			while (true)
			{
				if (EOF == fwscanf_s(pFile, L"%s", szRead, 255))
					break;

				wstring str = szRead;

				if (str == L"[ANIMATION_NAME]")
				{
					fwscanf_s(pFile, L"%s", szRead, 255);
				}
				else if (str == L"[ATLAS_TEXTURE]")
				{

					fwscanf_s(pFile, L"%s", szRead, 255);

					fwscanf_s(pFile, L"%s", szRead, 255);

					//�̰� ���� ã�°�
					result.Path = szRead;

				}
			}
			fclose(pFile);
			//�������

			// ã���� �ϴ� ��θ� ������ �������� ���� strContentPath ���� �ؽ���\\ ������ ��ΰ� �������
			size_t pos = fullPath.find(StrContentPath);

			if (pos != wstring::npos)
			{
				// ��� ���� �������� ������������ ����
				StrContentPath = CPathMgr::Get()->GetContentPath();
				// 'StrContentPath' ������ ��θ� �����
				fullPath = fullPath.substr(pos + StrContentPath.length());
			}

			CAnimator* Ani = m_AniObj->GetEditAnimator();
			Ani->LoadAnimation(fullPath);
			
			// fullPath�� ����� ��ο��� ���ϸ� ����
			wstring fileName = fullPath.substr(fullPath.find_last_of(L"\\") + 1);

			// ���ϸ��� Ȯ������ .anim ����
			size_t dotPos = fileName.find(L".anim");

			if (dotPos != wstring::npos)
			{
				fileName = fileName.substr(0, dotPos);
			}

			// ã�� ���ϸ��� ���� Ű���� �Է��ؼ� �Ʊ� �ε��� ������ �ִϸ��̼��� �������
			Ani->Play(fileName,true);
			result.Name = fileName;
			return result;
		}
	}

	// �Լ��� ��ȯ Ÿ���� wstring �̹Ƿ� ��� ���ǿ� �ɸ��� ���� ��� �⺻���� �� ���ڿ� ��ȯ
	return result;
}



void CLevel_Editor::SaveAni(wstring _Path, wstring _Name)
{
	CAnimator* Ani = m_AniObj->GetEditAnimator();
	wstring pluspath = L"animation\\";
	pluspath += _Path;

	Ani->SaveEditAnimation(pluspath, _Name);
}

void CLevel_Editor::EditPlay(const wstring _Name)
{
	CAnimator* Ani = m_AniObj->GetEditAnimator();
	Ani->Play(_Name, true);
}

void CLevel_Editor::EditStop()
{
	CAnimator* Ani = m_AniObj->GetEditAnimator();
	Ani->Stop();
}



void CLevel_Editor::FrmAdd(Vec2 _LeftTop, Vec2 _Offset, Vec2 _Slice, float _Duration, const wstring& _animName)
{
	//���� ���� �ڵ� ������ �߰� ���� 
	tAnimFrm ADD = {};
	ADD.vLeftTop = _LeftTop;
	ADD.vOffset = _Offset;
	ADD.vSlice = _Slice;
	ADD.Duration = _Duration;

	CAnimator* Ani = m_AniObj->GetEditAnimator();
	Ani->AddFrameToAnimation(_animName, ADD);
}

void CLevel_Editor::FrmSave(Vec2 _LeftTop, Vec2 _Offset, Vec2 _Slice, float _Duration, const wstring& _animName ,int _idx)
{
	// Ư�� ������ ã�Ƽ� ���� ���
	CAnimator* Ani = m_AniObj->GetEditAnimator();

	CAnim* FrmIdx = Ani->FindAnimation(_animName);
	
	FrmIdx->GetFrame(_idx).vOffset = _Offset;
	FrmIdx->GetFrame(_idx).vLeftTop = _LeftTop;
	FrmIdx->GetFrame(_idx).vSlice = _Slice;
	FrmIdx->GetFrame(_idx).Duration = _Duration;
}

void CLevel_Editor::DurationSave( const wstring& _animName ,float _Duration)
{
	CAnimator* Ani = m_AniObj->GetEditAnimator();
	CAnim* FrmIdx = Ani->FindAnimation(_animName);

	map<wstring, CAnim*> AniMap = Ani->GetMapAnimator();

	int IDX = 0;

	auto iter = AniMap.find(_animName);

	if (iter != AniMap.end())
	{
		IDX = iter->second->m_vecFrm.size(); // �ش� ��ü�� ���� ũ�� ��ȯ (0-based index)
		int a = 0;

		for (int i = 0; IDX > i; ++i)
		{
			FrmIdx->GetFrame(i).Duration = _Duration;
		}
	}
	else
	return;
}

INT_PTR CALLBACK MapProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK)
		{
			// 1. ���� ������ �����ؾ��Ѵ� GetCurrentLevel�� ���������ͷ� �ִϱ� ������ ������ �ٿ�ĳ������ �ؼ� ����Ѵ�.
			CLevel* pCurLevel = CLevelMgr::Get()->GetCurrentLevel();
			CLevel_Editor* pEditorLevel = dynamic_cast<CLevel_Editor*>(pCurLevel);

			// ��ư � �Է��� ���� ���� �������� �Լ� 
			// ������� ���̵� �˷���
			int Minx = GetDlgItemInt(hWnd, IDC_Size_Min_X, nullptr, true);
			int Miny = GetDlgItemInt(hWnd, IDC_Size_Min_Y, nullptr, true);
			int Maxx = GetDlgItemInt(hWnd, IDC_Size_Max_X, nullptr, true);
			int Maxy = GetDlgItemInt(hWnd, IDC_Size_Max_Y, nullptr, true);

			const int TILE_SIZE = 64;

			// ���� Ÿ�� ��ġ ��� (�ø�)
			int startTileX = static_cast<int>(std::floor(Minx / TILE_SIZE));
			int startTileY = static_cast<int>(std::floor(Miny / TILE_SIZE));

			// �� Ÿ�� ��ġ ��� (�ø�)
			int endTileX = static_cast<int>(std::ceil(Maxx / TILE_SIZE));
			int endTileY = static_cast<int>(std::ceil(Maxy / TILE_SIZE));

			// Ÿ�� ���� ���
			int tileCountX = endTileX - startTileX;
			int tileCountY = endTileY - startTileY;

			CTileMap* pTileMap = pEditorLevel->GetTileMap();
			pTileMap->SetRowCol(tileCountX, tileCountY);
			

			// ���� ������ ������ �ƴѰ����� �޴��ٰ� ȣ�� �ƴٸ� ���� ������  �� ����Ʈ�� ��������״� ���Ʈ�� �Ǵ�.
			assert(pEditorLevel);

			
			return (INT_PTR)TRUE;
		}
		else if (LOWORD(wParam) == IDC_Map_Save)
		{
			CLevel_Editor* pLevel = dynamic_cast<CLevel_Editor*>(CLevelMgr::Get()->GetCurrentLevel());
			assert(pLevel);
			pLevel->SavePlatform();
		}

		else if (LOWORD(wParam) == IDC_Map_Load)
		{
			CLevel_Editor* pLevel = dynamic_cast<CLevel_Editor*>(CLevelMgr::Get()->GetCurrentLevel());
			assert(pLevel);
			pLevel->LoadPlatform();
		}

		else if (LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hWnd, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}

		break;
	}
	return (INT_PTR)FALSE;
}

void CLevel_Editor::SaveMap(const wstring& _FolderPath)
{
	wstring strName = GetName();
	wstring FilePath = _FolderPath + strName + L".map";

	FILE* pFile = nullptr;
	errno_t err = _wfopen_s(&pFile, FilePath.c_str(), L"w");

	if (err != 0 || !pFile)
	{
		wchar_t szError[256];
		_wcserror_s(szError, 256, err);
		wstring errorMsg = L"Failed to open file for writing: " + wstring(szError);
		MessageBox(NULL, errorMsg.c_str(), L"Error", MB_ICONERROR);
		return;
	}

	if (m_vecEditMap.empty())
	{
		MessageBox(NULL, L"No map data to save!", L"Warning", MB_ICONWARNING);
		fclose(pFile);
		return;
	}

	// �÷��� ������
	fwprintf(pFile, L"[PLATFORM_DATA]\n");
	fwprintf(pFile, L"PLATFORM_COUNT %d\n\n", static_cast<int>(m_vecEditMap.size()));

	for (size_t i = 0; i < m_vecEditMap.size(); ++i)
	{
		fwprintf(pFile, L"PLATFORM %d\n", static_cast<int>(i));
		fwprintf(pFile, L"Type        %d\n", m_vecEditMap[i].Type);
		fwprintf(pFile, L"Position    %f %f\n", m_vecEditMap[i].StartPos.x, m_vecEditMap[i].StartPos.y);
		fwprintf(pFile, L"Scale       %f %f\n\n", m_vecEditMap[i].Scale.x, m_vecEditMap[i].Scale.y);
	}

	fclose(pFile);

}

void CLevel_Editor::LoadMap(const wstring& _FilePath)
{
	FILE* pFile = nullptr;
	errno_t err = _wfopen_s(&pFile, _FilePath.c_str(), L"r");
	if (err != 0 || !pFile)
	{
		wchar_t szError[256];
		_wcserror_s(szError, 256, err);
		wstring errorMsg = L"Failed to open file for reading: " + wstring(szError);
		MessageBox(NULL, errorMsg.c_str(), L"Error", MB_ICONERROR);
		return;
	}

	// ���� ������Ʈ ����
	DeleteAllObjects();

	wchar_t szBuffer[256] = {};
	int platformCount = 0;

	// [PLATFORM_DATA] �±� ã��
	while (fwscanf_s(pFile, L"%s", szBuffer, (unsigned)_countof(szBuffer)) != EOF)
	{
		if (wcscmp(szBuffer, L"[PLATFORM_DATA]") == 0)
		{
			fwscanf_s(pFile, L"%s %d", szBuffer, (unsigned)_countof(szBuffer), &platformCount);
			break;
		}
	}

	for (int i = 0; i < platformCount; ++i)
	{
		int type, platformIndex;
		Vec2 startPos, scale;

		// �� �÷��� ������ �б�
		fwscanf_s(pFile, L"%s %d", szBuffer, (unsigned)_countof(szBuffer), &platformIndex);
		fwscanf_s(pFile, L"%s %d", szBuffer, (unsigned)_countof(szBuffer), &type);
		fwscanf_s(pFile, L"%s %f %f", szBuffer, (unsigned)_countof(szBuffer), &startPos.x, &startPos.y);
		fwscanf_s(pFile, L"%s %f %f", szBuffer, (unsigned)_countof(szBuffer), &scale.x, &scale.y);

		// ��ü ���� �� �߰�
		CObj* pObj = nullptr;
		LAYER_TYPE layerType;

		if (type == 1) // �÷���
		{
			pObj = new CPlatform;
			pObj->SetName(L"Platform");
			layerType = LAYER_TYPE::PLATFORM;
		}
		else if (type == 2) // �÷ξ�
		{
			pObj = new Cfloor;
			pObj->SetName(L"Floor");
			layerType = LAYER_TYPE::FLOOR;
		}

		if (pObj)
		{
			pObj->SetPos(startPos);
			pObj->SetScale(scale);
			AddObject(pObj, layerType);
		}
	}

	fclose(pFile);
	MessageBox(NULL, L"Map loaded successfully!", L"Success", MB_ICONINFORMATION);
}

void CLevel_Editor::SavePlatform()
{
	// ��� ����ְ�
	wstring StrContentPath = CPathMgr::Get()->GetContentPath();
	StrContentPath += L"platform\\";
	wchar_t szFilePath[255] = {}; // �迭 �ʱ�ȭ

	OPENFILENAME Desc = {}; // ���� ����ü �ʱ�ȭ
	Desc.lStructSize = sizeof(OPENFILENAME);
	Desc.hwndOwner = nullptr;
	Desc.lpstrFile = szFilePath;
	Desc.nMaxFile = 255;
	Desc.lpstrFilter = L"Platform\0*.platform\0ALL\0*.*";
	Desc.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;
	Desc.lpstrInitialDir = StrContentPath.c_str();

	if (GetSaveFileName(&Desc))
	{
		if (wcslen(szFilePath) != 0)
		{
			// SaveMap �Լ� ȣ��
			SaveMap(szFilePath);
		}
	}

	CUIMgr::Get()->SetPreventFrame(1);
}

void CLevel_Editor::LoadPlatform()
{
	// ��� ����ְ�
	wstring StrContentPath = CPathMgr::Get()->GetContentPath();
	StrContentPath += L"platform\\";
	wchar_t szFilePath[255] = {}; // �迭 �ʱ�ȭ

	OPENFILENAME Desc = {}; // ���� ����ü �ʱ�ȭ
	Desc.lStructSize = sizeof(OPENFILENAME);
	Desc.hwndOwner = nullptr;
	Desc.lpstrFile = szFilePath;
	Desc.nMaxFile = 255;
	Desc.lpstrFilter = L"Platform\0*.platform\0ALL\0*.*";
	Desc.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	Desc.lpstrInitialDir = StrContentPath.c_str();

	if (GetOpenFileName(&Desc))
	{
		if (wcslen(szFilePath) != 0)
		{
			// LoadMap �Լ� ȣ��
			LoadMap(szFilePath);
		}
	}

	CUIMgr::Get()->SetPreventFrame(1);
}