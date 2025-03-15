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


// 그냥 전방선언
LRESULT CALLBACK TileProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

// 함수 포인터 사용 예시창
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
	// 메뉴 추가	
	SetMenu(CEngine::Get()->GetMainHwnd(), m_hMenu);



	// 메뉴가 붙었으니까, 다시 윈도우 크기 재계산
	UINT width = CEngine::Get()->GetResolution().x;
	UINT height = CEngine::Get()->GetResolution().y;
	CEngine::Get()->ChangeWindowResolution((float)width, (float)height);

	CCamera::Get()->SetLookAt(Vec2( (float)width /2, (float)height/2));

	

	// 오브젝트 생성
	CObj* pObj = nullptr;

	 //Tile
	pObj = new CTile;
	pObj->SetName(L"Tile Object");
	pObj->SetPos(0.f, 0.f);
	m_TileMap = pObj->GetComponent<CTileMap>();
	m_TileMap->SetRowCol(0, 0);
	AddObject(pObj, LAYER_TYPE::TILE);

	// 에딧 오브젝트 시험
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
	

	// Panel 추가
	//CPanel* pPanel = new CPanel;
	//pPanel->SetScale(Vec2(500.f, 400.f));
	//pPanel->SetPos(Vec2(700.f, 30.f));
	//AddObject(pPanel, LAYER_TYPE::UI);


	// UI 추가하기
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
	// 메뉴 제거
	SetMenu(CEngine::Get()->GetMainHwnd(), nullptr);

	// 메뉴가 제거되었으니, 윈도우 크기 다시 계산
	UINT width = CEngine::Get()->GetResolution().x;
	UINT height = CEngine::Get()->GetResolution().y;
	CEngine::Get()->ChangeWindowResolution(width, height);



	// 오브젝트 삭제
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

	// 애니메이션 관련 마우스체크
	AniMouseMode();
	
	
	// 타일 관련 마우스 체크
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

		// AnProc로 메시지 보내기
		SendMessage(hDialog, WM_COMMAND, MAKEWPARAM(IDC_BTEST, 0), 0);
	}

}


void CLevel_Editor::TileMode()
{
	Vec2 vvMousePos = CKeyMgr::Get()->GetMousePos();
	vvMousePos = CCamera::Get()->GetRealPos(vvMousePos);

	// 1. 내 마우스 좌표를 구해서 그 위치를 선으로 인식하면
	// 2. 타일 사이즈로 나누면 몇 행 몇 열인지가 나오니까 그 값을 이용해서
	// 3. GetTileInfo에 넣어서 그 마우스가 있는 타일의 정보를 조작함

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


	//마우스 클릭으로 플랫폼 및 플로어 만들기
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

			// 벡터에 정보저장
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

			// 벡터에 정보저장
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

			// 벡터에 정보저장
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
//		// 마우스 버튼을 눌렀을 때의 위치를 저장하고 드래깅 시작
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
//			// 드래그한 영역의 최소/최대 좌표를 구합니다.
//			float fMinX = min(vvMouseStartPos.x, vvMousePos.x);
//			float fMaxX = max(vvMouseStartPos.x, vvMousePos.x);
//			float fMinY = min(vvMouseStartPos.y, vvMousePos.y);
//			float fMaxY = max(vvMouseStartPos.y, vvMousePos.y);
//
//			// 시작/끝 좌표를 타일 인덱스로 변환합니다.
//			UINT StartCol = fMinX / TileSize.x;
//			UINT EndCol = fMaxX / TileSize.x;
//			UINT StartRow = fMinY / TileSize.y;
//			UINT EndRow = fMaxY / TileSize.y;
//
//			// 드래그한 영역의 모든 타일을 업데이트합니다.
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
//			// 드래깅 종료
//			bDragging = false;
//		}
//	}
//}

void CLevel_Editor::SaveTile()
{

	// 경로 얻어주고
	wstring StrContentPath = CPathMgr::Get()->GetContentPath();
	StrContentPath += L"tile\\";

	wchar_t szFilePath[255] = {}; // 배열 초기화

	OPENFILENAME Desc = {}; // 파일 구조체 초기화
	Desc.lStructSize = sizeof(OPENFILENAME); //사이즈
	Desc.hwndOwner = nullptr; // 별 안중요함
	Desc.lpstrFile = szFilePath; // 아까 초기화한 배열 넣어줌
	Desc.nMaxFile = 255; //우리가 목적지로 설정한 배열의 최대 갯수
	// 이건 필터를 추가하는건데 타일 파일에 들어갔을시 * 이름은 상관없다는 뜻
	// 하지만 확장자명이 .tile 이여야하고 ALL 모든 정보를 저장하고 *.* 모든게 상관없음
	Desc.lpstrFilter = L"Tile\0*.tile\0ALL\0*.*";
	Desc.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST; // 이건 고정
	Desc.lpstrInitialDir = StrContentPath.c_str();
	// 파일열었을시 경로를 설정해줌

		

	// 윈도우가 다 설계한 모양으로 윈도우 하나를 만든다 탐색기 창
	// 그리고 거기에 옵션을 집어넣으면 된다.
	// 반환값으로 확인을 눌렀는지 , 취소를 눌렀는지 를 확인할수도 있다.
	// 물론 다른것도 가능하겄지 그럼
	// 이 함수의 기능은 우리가 설정한 곳의 경로만을 알려줄 뿐이다.
	if (GetSaveFileName(&Desc))
	{
		if (wcslen(szFilePath) != 0)    // 선택한 경로가 있을 시만 세이브를 진행하게 예외처리. (파일을 선택했을시)
		{
			m_TileMap->Save(szFilePath);
		}
	}

	CUIMgr::Get()->SetPreventFrame(1);
}

void CLevel_Editor::LoadTile()
{

	// 경로 얻어주고

	wstring StrContentPath = CPathMgr::Get()->GetContentPath();
	StrContentPath += L"tile\\";

	wchar_t szFilePath[255] = {}; // 배열 초기화

	OPENFILENAME Desc = {}; // 파일 구조체 초기화
	Desc.lStructSize = sizeof(OPENFILENAME); //사이즈
	Desc.hwndOwner = nullptr; // 별 안중요함
	Desc.lpstrFile = szFilePath; // 아까 초기화한 배열 넣어줌
	Desc.nMaxFile = 255; //우리가 목적지로 설정한 배열의 최대 갯수
	// 이건 필터를 추가하는건데 타일 파일에 들어갔을시 * 이름은 상관없다는 뜻
	// 하지만 확장자명이 .tile 이여야하고 ALL 모든 정보를 저장하고 *.* 모든게 상관없음
	Desc.lpstrFilter = L"ALL\0*.*";
	Desc.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST; // 이건 고정
	Desc.lpstrInitialDir = StrContentPath.c_str();
	// 파일열었을시 경로를 설정해줌

	// 위에꺼랑 다른거는 열어주냐 저장하냐의 이 함수 차이뿐
	if (GetOpenFileName(&Desc))
	{
		if (wcslen(szFilePath) != 0)    // 선택한 경로가 있을 시만 로드를 진행하게 예외처리. (파일을 선택했을시)
		{
			m_TileMap->Load(szFilePath);
		}
	}

	CUIMgr::Get()->SetPreventFrame(1);
}



// 타일관련 메시지 처리기 프로시저
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
			// 버튼 등에 입력한 정수 값을 가져오는 함수 
			// 윈도우와 아이디를 알려줌
			int Col = GetDlgItemInt(hWnd, IDC_Col, nullptr, true);
			int Row = GetDlgItemInt(hWnd, IDC_Row, nullptr, true);

			if (Col == 0 || Row == 0)
			{
				MessageBox(nullptr,L"설정 오류", L"타일의 행,열은 0이 될수 없습니다", MB_OK);
				return (INT_PTR)TRUE;
			}

			// 1. 현재 레벨에 접근해야한다 GetCurrentLevel은 레벨포인터로 주니까 에디터 레벨로 다운캐스팅을 해서 사용한다.
			CLevel* pCurLevel = CLevelMgr::Get()->GetCurrentLevel();
			CLevel_Editor* pEditorLevel = dynamic_cast<CLevel_Editor*>(pCurLevel);

			// 만약 에디터 레벨이 아닌곳에서 메뉴바가 호출 됐다면 설계 오류고  널 포인트가 들어있을테니 어썰트를 건다.
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
	SendMessage(hComboBox, CB_SETCURSEL, 0, 0);  // 첫 번째 항목을 기본 선택

	// 콤보박스의 각 항목 높이를 가져옴
	int itemHeight = SendMessage(hComboBox, CB_GETITEMHEIGHT, 0, 0);

	// 콤보박스의 항목 수를 가져옴
	int itemCount = SendMessage(hComboBox, CB_GETCOUNT, 0, 0);

	// 드롭다운 목록 높이 계산
	int dropDownHeight = itemHeight * itemCount;

	// 콤보박스의 높이를 설정할 사각형을 얻음
	RECT rect;
	GetWindowRect(hComboBox, &rect);
	int comboWidth = rect.right - rect.left;

	// 드롭다운 목록의 높이를 설정하고 콤보박스를 업데이트
	SetWindowPos(hComboBox, HWND_TOP, 0, 0, comboWidth, dropDownHeight, SWP_NOMOVE | SWP_NOZORDER);
}
void InitSPIN(HWND hWnd, int _spin , int _Edit)
{
	HWND hSpin = GetDlgItem(hWnd, _spin);
	HWND hEdit = GetDlgItem(hWnd, _Edit);

	// 스핀 컨트롤의 범위 설정 (-10000부터 10000까지)
	SendMessage(hSpin, UDM_SETRANGE32, -1000, 10000);

	// 스핀 컨트롤과 에디트 컨트롤 연결
	SendMessage(hSpin, UDM_SETBUDDY, (WPARAM)hEdit, 0);

	// 스핀 컨트롤 스타일 설정 (UDS_ALIGNRIGHT로 설정하여 오른쪽 정렬)
	DWORD spinStyle = (DWORD)GetWindowLong(hSpin, GWL_STYLE);
	spinStyle &= ~UDS_WRAP;  // UDS_WRAP 스타일 제거
	spinStyle |= UDS_ALIGNRIGHT;  // 오른쪽 정렬 스타일 추가
	SetWindowLong(hSpin, GWL_STYLE, spinStyle);

	// 스핀 컨트롤의 스피드(스텝) 설정
	UDACCEL accel = { 0, 1 };  // {nSec, nInc} - nSec 초 후에 nInc 단위로 증가/감소
	SendMessage(hSpin, UDM_SETACCEL, 1, (LPARAM)&accel);

}


//float 값을 wstring로 변환해서 사용하는 코드
void SetDlgItemFloat(HWND _hWnd, int _nIDDlgItem, float _value)
{
	// float 값을 wstring으로 변환
	std::wstringstream wss;		   // wstringstream 객체 생성
	wss << _value;				   // float 값을 wstringstream에 삽입
	std::wstring wstr = wss.str(); // wstringstream을 wstring으로 변환

	// 변환된 문자열을 다이얼로그 컨트롤에 설정
	SetDlgItemText(_hWnd, _nIDDlgItem, wstr.c_str());
}

// 에디트 컨트롤에서 원래는 int로 가져오지만 문자 타입으로 가져와 float로 변환하는 함수
float GetDlgItemFloat(HWND _hWnd, int _nIDDlgItem)
{
	// 다이얼로그 컨트롤에서 텍스트를 가져오기
	wchar_t buffer[256]; // 충분히 큰 버퍼 사용
	GetDlgItemText(_hWnd, _nIDDlgItem, buffer, 256);

	// wstring을 float으로 변환
	float floatValue = 0.0f;
	std::wstringstream wss(buffer); // 버퍼로부터 wstringstream 객체 생성
	wss >> floatValue;              // wstringstream에서 float으로 변환

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
		{//레벨에디터 받아오기
		CLevel* pCurLevel = CLevelMgr::Get()->GetCurrentLevel();
		CLevel_Editor* pEditorLevel = dynamic_cast<CLevel_Editor*>(pCurLevel);
		// 만약 에디터 레벨이 아닌곳에서 메뉴바가 호출 됐다면 설계 오류고  널 포인트가 들어있을테니 어썰트를 건다.
		assert(pEditorLevel);


		//####################입력 버튼 ####################
		if (LOWORD(wParam) == IDC_BTEST)
		{

			// 마우스 좌표 계산
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

		//################### 미리보기 #################
		else if (LOWORD(wParam) == IDC_T_B)
		{

			float LX = GetDlgItemFloat(hWnd, IDC_Left_X);
			float LY = GetDlgItemFloat(hWnd, IDC_Left_Y);
			float SX = GetDlgItemFloat(hWnd, IDC_Slice_X);
			float SY = GetDlgItemFloat(hWnd, IDC_Slice_Y);
			float OX = GetDlgItemFloat(hWnd, IDC_OffSet_X);
			float OY = GetDlgItemFloat(hWnd, IDC_OffSet_Y);

			// 경로용
			TCHAR zzBuffer[100];
			GetDlgItemText(hWnd, IDC_Coice, zzBuffer, sizeof(zzBuffer) / sizeof(TCHAR));
			wstring Path = zzBuffer;

			CTexture* texture;

			texture = CAssetMgr::Get()->LoadTexture(Path, Path);

			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);

			// 픽쳐 컨트롤 핸들 가져오기
			HWND hPicture = GetDlgItem(hWnd, IDC_Ani_View);

			// 픽쳐 컨트롤의 DC 가져오기
			HDC hdcPicture = GetDC(hPicture);

			// 픽쳐 컨트롤의 클라이언트 영역 가져오기
			RECT rect;
			GetClientRect(hPicture, &rect);
			
			// 사용자 정의 브러시를 사용하여 픽쳐 컨트롤의 배경을 회색으로 지우기
			HBRUSH hBrush = CreateSolidBrush(RGB(100, 100, 100)); // RGB 값으로 회색 생성
			FillRect(hdcPicture, &rect, hBrush);
			// 브러시 삭제
			DeleteObject(hBrush);

			// 내가 드래그한 넓이와 높이
			UINT width = (UINT)SX;
			UINT height = (UINT)SY;

			// 중심 좌표 계산
			int centerX = rect.right / 2;
			int centerY = rect.bottom / 2;

			// 드래그된 크기를 적용한 그리기 위치 계산 (중심 기준)
			int x = centerX - width / 2 + (int)OX;
			int y = centerY - height / 2 + (int)OY;

			BLENDFUNCTION blend = {};
			blend.BlendOp = AC_SRC_OVER;
			blend.BlendFlags = 0;
			blend.SourceConstantAlpha = 255;
			blend.AlphaFormat = AC_SRC_ALPHA;
			AlphaBlend(hdcPicture, x, y, width, height, texture->GetDC(), (int)LX, (int)LY, width, height, blend);

			// 디버그 그리기용
			Vec2 rectVec2 = Vec2((float)rect.right, (float)rect.bottom);

			DrawDebugShape(DEBUG_SHAPE::CROSS, PEN_TYPE::GREEN, BRUSH_TYPE::HOLLOW, rectVec2, rectVec2);

			// 펜 선택
			SELECT_PEN(hdcPicture, PEN_TYPE::GREEN);
			// 브러쉬 선택
			SELECT_BRUSH(hdcPicture, BRUSH_TYPE::HOLLOW);

			float SDX = GetDlgItemFloat(hWnd, IDC_Scale_X);
			float SDY = GetDlgItemFloat(hWnd, IDC_Scale_Y);
			float SDO = GetDlgItemFloat(hWnd, IDC_Scale_O); // 콜라이더 오프셋용 보통 Y만 씀

			Rectangle(hdcPicture
				, rect.right / 2 - (int)SDX / 2
				, rect.bottom / 2 - (int)SDY / 2 + (int)SDO
				, rect.right / 2 + (int)SDX / 2
				, rect.bottom/2 + (int)SDY/2 + (int)SDO);

			EndPaint(hWnd, &ps);
		}

		//####################애니재생 ####################
		else if (LOWORD(wParam) == IDC_Play)
		{
			TCHAR azBuffer[100];
			GetDlgItemText(hWnd, IDC_Set_Name, azBuffer, sizeof(azBuffer) / sizeof(TCHAR));
			wstring Nmae = azBuffer;

			pEditorLevel->EditPlay(Nmae);
		}
		//####################애니 지우기 ####################
		else if (LOWORD(wParam) == IDC_Stop)
		{
			// 이름 설정용
			TCHAR azBuffer[100];
			GetDlgItemText(hWnd, IDC_Set_Name, azBuffer, sizeof(azBuffer) / sizeof(TCHAR));
			wstring Nmae = azBuffer;

			CAnimator* Ani = pEditorLevel->GetEditObj()->GetEditAnimator();
			Ani->Stop();
			Ani->DeleteAnimation(Nmae);

		}

		//####################애니 생성 ####################
		else if (LOWORD(wParam) == IDC_Ani_Creat)
		{
			float LX = GetDlgItemFloat(hWnd, IDC_Left_X);
			float LY = GetDlgItemFloat(hWnd, IDC_Left_Y);
			float SX = GetDlgItemFloat(hWnd, IDC_Slice_X);
			float SY = GetDlgItemFloat(hWnd, IDC_Slice_Y);
			float OX = GetDlgItemFloat(hWnd, IDC_OffSet_X);
			float OY = GetDlgItemFloat(hWnd, IDC_OffSet_Y);
			float DU = GetDlgItemFloat(hWnd, IDC_Duration);


			// 경로용
			TCHAR zzBuffer[100];
			GetDlgItemText(hWnd, IDC_Coice, zzBuffer, sizeof(zzBuffer) / sizeof(TCHAR));
			wstring Path = zzBuffer;

			// 이름 설정용
			TCHAR azBuffer[100];
			GetDlgItemText(hWnd, IDC_Set_Name, azBuffer, sizeof(azBuffer) / sizeof(TCHAR));
			wstring Nmae = azBuffer;

			pEditorLevel->CreatAni(Nmae, Path, Nmae, 1, 1.f, Vec2(SX, SY), Vec2(LX, LY));

			//다이얼로그 안끔
			return (INT_PTR)TRUE;
		}

		//####################텍스쳐파일 고르기 ####################
		else if (LOWORD(wParam) == IDC_ATLAS_Select) // 아틀라스 셀렉트 누르기
		{
			wstring FileName = pEditorLevel->SelectAtlas();


			SetDlgItemText(hWnd, IDC_Coice, FileName.c_str());

			//다이얼로그 안끔
			return (INT_PTR)TRUE;
		}

		//####################저장된 애니메이션 불러오기 ####################
		else if (LOWORD(wParam) == IDC_ATLAS_LOAD) // 아틀라스 로드 누르기
		{

			AniNamePath NamePath = pEditorLevel->LoadAni();

			SetDlgItemText(hWnd, IDC_Atlas_Name, NamePath.Name.c_str());

			SetDlgItemText(hWnd, IDC_Coice, NamePath.Path.c_str());

			//다이얼로그 끄기 
			return (INT_PTR)TRUE;
		}

		//#################### 만든 애니메이션 저장하기 ####################
		else if (LOWORD(wParam) == IDC_ANI_SAVE) // 애니 세이브 누르기
		{

			// 콤보박스 핸들 가져오기
			HWND hComboBox = GetDlgItem(hWnd, IDC_Savelist);
			// 선택된 항목의 인덱스를 가져오기
			int index = SendMessage(hComboBox, CB_GETCURSEL, 0, 0);
			// 선택된 항목의 텍스트를 가져오기 위한 버퍼
			wchar_t selectedText[256] = {};
			// 선택된 항목의 텍스트 가져오기
			SendMessage(hComboBox, CB_GETLBTEXT, index, (LPARAM)selectedText);
			// wchar_t 배열을 wstring으로 변환
			wstring selectedString = selectedText;

			// 이름 설정용
			TCHAR azBuffer[100];
			GetDlgItemText(hWnd, IDC_Set_Name, azBuffer, sizeof(azBuffer) / sizeof(TCHAR));
			wstring Name = azBuffer;

			//selectedString 자리에는 어떤 폴더에 저장할건지 Name 는 내가 설정한 이름
			pEditorLevel->SaveAni(selectedString ,Name);
			{
				wstring Logg = Name + L"가" + selectedString + L"에 저장 되었습니다.";
				MessageBox(nullptr, Logg.c_str(), L"저장 완료", MB_OK);
				return (INT_PTR)TRUE;

			}


			//다이얼로그 끄기 
			return (INT_PTR)TRUE;
		}

		//####################프레임 하나 추가하기 ####################
		else if (LOWORD(wParam) == IDC_Frm_Add) // 프레임 추가 문제는 이제 마우스 클릭하면 가능하게 해야함
		{
			// 처음에 구현했던 모달리스 창 만들기
			//HWND hDialog = nullptr;
			//hDialog = CreateDialog(g_hInst, MAKEINTRESOURCE(IDD_FrmAdd), hWnd, FrmProc);
			//ShowWindow(hDialog, SW_SHOW);

			// 마우스 좌표 계산
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

			// 이름 설정용
			TCHAR azBuffer[100];
			GetDlgItemText(hWnd, IDC_Set_Name, azBuffer, sizeof(azBuffer) / sizeof(TCHAR));
			wstring Nmae = azBuffer;


			pEditorLevel->FrmAdd(Vec2(LX, LY), Vec2(OX, OY), Vec2(SX, SY), DU, Nmae);

			return (INT_PTR)TRUE;
		}

		//################# 프레임 수정하기 ###################
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

			// 이름 설정용
			TCHAR azBuffer[100];
			GetDlgItemText(hWnd, IDC_Set_Name, azBuffer, sizeof(azBuffer) / sizeof(TCHAR));
			wstring Nmae = azBuffer;
	
			pEditorLevel->FrmSave(Vec2(LX, LY), Vec2(OX, OY), Vec2(SX, SY), DU, Nmae, idx);

		}

		//################## 듀레이션 일괄 적용 ################
		else if (LOWORD(wParam) == IDC_Duration_B)
		{
			float DU = GetDlgItemFloat(hWnd, IDC_Duration);

			// 이름 설정용
			TCHAR azBuffer[100];
			GetDlgItemText(hWnd, IDC_Set_Name, azBuffer, sizeof(azBuffer) / sizeof(TCHAR));
			wstring Nmae = azBuffer;

			pEditorLevel->DurationSave(Nmae, DU);

		}

		
		//####################확인 버튼 ####################
		else if (LOWORD(wParam) == IDOK)
		{
			EndDialog(hWnd, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}

		//####################나가기 ~ ####################
		else if (LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hWnd, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
	}
		break;

	//###################### 스핀 컨트롤#################
	case WM_NOTIFY:
		{
		// 스핀컨트롤용 변수 설정
		long long wmId = LOWORD(wParam);
		long long wmEvent = HIWORD(wParam);

		long long controlId = LOWORD(wParam);  // 컨트롤의 ID를 얻습니다.
		NMHDR* nmhdr = (NMHDR*)lParam;  // NMHDR 구조체를 얻습니다.

		// 스핀 컨트롤에서 발생한 알림인지 확인합니다.
		if (controlId == IDC_SPIN1 && nmhdr->code == UDN_DELTAPOS)
		{
			// NMUPDOWN 구조체로부터 스핀 컨트롤의 변경 정보를 얻습니다.
			NMUPDOWN* nmud = (NMUPDOWN*)lParam;

			// 현재 에디트 컨트롤의 값을 가져옵니다.
			long long currentValue = GetDlgItemFloat(hWnd, IDC_Left_X);

			// 변경된 값을 계산하여 에디트 컨트롤에 설정합니다.
			long long newValue = currentValue + nmud->iDelta;

			SetDlgItemFloat(hWnd, IDC_Left_X, (float)newValue);

			SendMessage(hWnd, WM_COMMAND, MAKEWPARAM(IDC_T_B, 0), 0);
		}

		else if (controlId == IDC_SPIN2 && nmhdr->code == UDN_DELTAPOS)
		{
			// NMUPDOWN 구조체로부터 스핀 컨트롤의 변경 정보를 얻습니다.
			NMUPDOWN* nmud = (NMUPDOWN*)lParam;

			// 현재 에디트 컨트롤의 값을 가져옵니다.
			long long currentValue = GetDlgItemFloat(hWnd, IDC_Left_Y);

			// 변경된 값을 계산하여 에디트 컨트롤에 설정합니다.
			long long newValue = currentValue + nmud->iDelta;

			SetDlgItemFloat(hWnd, IDC_Left_Y, (float)newValue);

			SendMessage(hWnd, WM_COMMAND, MAKEWPARAM(IDC_T_B, 0), 0);
		}

		else if (controlId == IDC_SPIN3 && nmhdr->code == UDN_DELTAPOS)
		{
			// NMUPDOWN 구조체로부터 스핀 컨트롤의 변경 정보를 얻습니다.
			NMUPDOWN* nmud = (NMUPDOWN*)lParam;

			// 현재 에디트 컨트롤의 값을 가져옵니다.
			long long currentValue = GetDlgItemFloat(hWnd, IDC_OffSet_X);

			// 변경된 값을 계산하여 에디트 컨트롤에 설정합니다.
			long long newValue = currentValue + nmud->iDelta;

			SetDlgItemFloat(hWnd, IDC_OffSet_X, (float)newValue);

			SendMessage(hWnd, WM_COMMAND, MAKEWPARAM(IDC_T_B, 0), 0);
		}

		else if (controlId == IDC_SPIN4 && nmhdr->code == UDN_DELTAPOS)
		{
			// NMUPDOWN 구조체로부터 스핀 컨트롤의 변경 정보를 얻습니다.
			NMUPDOWN* nmud = (NMUPDOWN*)lParam;

			// 현재 에디트 컨트롤의 값을 가져옵니다.
			long long currentValue = GetDlgItemFloat(hWnd, IDC_OffSet_Y);

			// 변경된 값을 계산하여 에디트 컨트롤에 설정합니다.
			long long newValue = currentValue + nmud->iDelta;

			SetDlgItemFloat(hWnd, IDC_OffSet_Y, (float)newValue);

			SendMessage(hWnd, WM_COMMAND, MAKEWPARAM(IDC_T_B, 0), 0);
		}

		else if (controlId == IDC_SPIN5 && nmhdr->code == UDN_DELTAPOS)
		{
			// NMUPDOWN 구조체로부터 스핀 컨트롤의 변경 정보를 얻습니다.
			NMUPDOWN* nmud = (NMUPDOWN*)lParam;

			// 현재 에디트 컨트롤의 값을 가져옵니다.
			long long currentValue = GetDlgItemFloat(hWnd, IDC_Slice_X);

			// 변경된 값을 계산하여 에디트 컨트롤에 설정합니다.
			long long newValue = currentValue + nmud->iDelta;

			SetDlgItemFloat(hWnd, IDC_Slice_X, (float)newValue);

			SendMessage(hWnd, WM_COMMAND, MAKEWPARAM(IDC_T_B, 0), 0);
		}

		else if (controlId == IDC_SPIN6 && nmhdr->code == UDN_DELTAPOS)
		{
			// NMUPDOWN 구조체로부터 스핀 컨트롤의 변경 정보를 얻습니다.
			NMUPDOWN* nmud = (NMUPDOWN*)lParam;

			// 현재 에디트 컨트롤의 값을 가져옵니다.
			long long currentValue = GetDlgItemFloat(hWnd, IDC_Slice_Y);

			// 변경된 값을 계산하여 에디트 컨트롤에 설정합니다.
			long long newValue = currentValue + nmud->iDelta;

			SetDlgItemFloat(hWnd, IDC_Slice_Y, (float)newValue);

			SendMessage(hWnd, WM_COMMAND, MAKEWPARAM(IDC_T_B, 0), 0);
		}

		else if (controlId == IDC_SPIN7 && nmhdr->code == UDN_DELTAPOS)
		{
			// NMUPDOWN 구조체로부터 스핀 컨트롤의 변경 정보를 얻습니다.
			NMUPDOWN* nmud = (NMUPDOWN*)lParam;

			// 현재 에디트 컨트롤의 값을 가져옵니다.
			long long currentValue = GetDlgItemFloat(hWnd, IDC_Scale_X);

			// 변경된 값을 계산하여 에디트 컨트롤에 설정합니다.
			long long newValue = currentValue + nmud->iDelta;

			SetDlgItemFloat(hWnd, IDC_Scale_X, (float)newValue);

			SendMessage(hWnd, WM_COMMAND, MAKEWPARAM(IDC_T_B, 0), 0);
		}

		else if (controlId == IDC_SPIN8 && nmhdr->code == UDN_DELTAPOS)
		{
			// NMUPDOWN 구조체로부터 스핀 컨트롤의 변경 정보를 얻습니다.
			NMUPDOWN* nmud = (NMUPDOWN*)lParam;

			// 현재 에디트 컨트롤의 값을 가져옵니다.
			long long currentValue = GetDlgItemFloat(hWnd, IDC_Scale_Y);

			// 변경된 값을 계산하여 에디트 컨트롤에 설정합니다.
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
				IDX = iter->second->m_vecFrm.size() - 1; // 해당 객체의 벡터 크기 반환 (0-based index)
			}
			else
			{
				return 0; // 키를 찾지 못한 경우 0 반환 (또는 예외 처리)
			}

			// 스핀 컨트롤의 범위 설정
			SendMessage(hSpin, UDM_SETRANGE32, 0, IDX);

			// 스핀 컨트롤의 초기값 설정
			SendMessage(hSpin, UDM_SETPOS32, 0, 0);

			// 스핀 컨트롤과 에디트 컨트롤 연결
			SendMessage(hSpin, UDM_SETBUDDY, (WPARAM)hEdit, 0);

			// 스핀 컨트롤 스타일 설정 (UDS_WRAP로 설정하여 범위를 초과하면 처음 값으로 되돌아가게 함)
			DWORD spinStyle = (DWORD)GetWindowLong(hSpin, GWL_STYLE);
			spinStyle |= UDS_WRAP;  // UDS_WRAP 스타일 추가
			spinStyle |= UDS_ALIGNRIGHT;  // 오른쪽 정렬 스타일 추가
			SetWindowLong(hSpin, GWL_STYLE, spinStyle);

			// 스핀 컨트롤의 스피드(스텝) 설정
			UDACCEL accel = { 0, 1 };  // {nSec, nInc} - nSec 초 후에 nInc 단위로 증가/감소
			SendMessage(hSpin, UDM_SETACCEL, 1, (LPARAM)&accel);

			// NMUPDOWN 구조체로부터 스핀 컨트롤의 변경 정보를 얻습니다.
			NMUPDOWN* nmud = (NMUPDOWN*)lParam;

			// 현재 에디트 컨트롤의 값을 가져옵니다.
			BOOL good;
			int currentValue = GetDlgItemInt(hWnd, IDC_Frm_Count, &good, TRUE);

			// 변경된 값을 계산합니다.
			int newValue = currentValue + nmud->iDelta;

			// IDX 범위 내에서 유지하기 위해 나머지 연산을 사용합니다.
			if (newValue < 0)
				newValue = IDX;
			else if (newValue > IDX)
				newValue = 0;

			// 에디트 컨트롤에 설정합니다.
			SetDlgItemInt(hWnd, IDC_Frm_Count, newValue, TRUE);


			// 아래 부터는 프레임 정보 읽어와서 칸에 채우기
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
	// 경로 얻어주고
	wstring StrContentPath = CPathMgr::Get()->GetContentPath();
	//StrContentPath += L"texture\\";

	wchar_t szFilePath[255] = {}; // 배열 초기화

	OPENFILENAME Desc = {}; // 파일 구조체 초기화
	Desc.lStructSize = sizeof(OPENFILENAME); //사이즈
	Desc.hwndOwner = nullptr; // 별 안중요함
	Desc.lpstrFile = szFilePath; // 아까 초기화한 배열 넣어줌
	Desc.nMaxFile = 255; //우리가 목적지로 설정한 배열의 최대 갯수
	// 이건 필터를 추가하는건데 타일 파일에 들어갔을시 * 이름은 상관없다는 뜻
	// 하지만 확장자명이 .tile 이여야하고 ALL 모든 정보를 저장하고 *.* 모든게 상관없음
	Desc.lpstrFilter = L"PNG\0*.png\0ALL\0*.*";
	Desc.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST; // 이건 고정
	Desc.lpstrInitialDir = StrContentPath.c_str();
	// 파일열었을시 경로를 설정해줌

	// 윈도우가 다 설계한 모양으로 윈도우 하나를 만든다 탐색기 창
	// 그리고 거기에 옵션을 집어넣으면 된다.
	// 반환값으로 확인을 눌렀는지 , 취소를 눌렀는지 를 확인할수도 있다.
	// 물론 다른것도 가능하겄지 그럼
	// 이 함수의 기능은 우리가 설정한 곳의 경로만을 알려줄 뿐이다.
	if (GetSaveFileName(&Desc))
	{
		if (wcslen(szFilePath) != 0)
		{
			// 총 파일 길이
			wstring fullPath = szFilePath;

			// 찾고자 하는 경로를 제외한 나머지를 제거 strContentPath 에는 텍스쳐\\ 까지의 경로가 들어있음
			size_t pos = fullPath.find(StrContentPath);

			if (pos != wstring::npos)
			{
				// 'StrContentPath' 이후의 경로만 남기기
				fullPath = fullPath.substr(pos + StrContentPath.length());

				// 먼저 텍스쳐 불러오고
				m_EditObj->SetTexture(CAssetMgr::Get()->LoadEditTexture(fullPath,szFilePath));


				// 받아낸 이름 반환해주기
				return fullPath;
			}

		}
	}

	// 함수의 반환 타입이 wstring 이므로 모든 조건에 걸리지 않을 경우 기본값인 빈 문자열 반환
	return L"";
}

AniNamePath CLevel_Editor::LoadAni()
{
	// 반환해줄 구조체 설정
	AniNamePath result = {};

	// 경로 얻어주고
	wstring StrContentPath = CPathMgr::Get()->GetContentPath();
	StrContentPath += L"animation\\";
	wchar_t szFilePath[255] = {}; // 배열 초기화

	OPENFILENAME Desc = {}; // 파일 구조체 초기화
	Desc.lStructSize = sizeof(OPENFILENAME); //사이즈
	Desc.hwndOwner = nullptr; // 별 안중요함
	Desc.lpstrFile = szFilePath; // 아까 초기화한 배열 넣어줌
	Desc.nMaxFile = 255; //우리가 목적지로 설정한 배열의 최대 갯수

	// 이건 필터를 추가하는건데 타일 파일에 들어갔을시 * 이름은 상관없다는 뜻
	// 하지만 확장자명이 .tile 이여야하고 ALL 모든 정보를 저장하고 *.* 모든게 상관없음
	Desc.lpstrFilter = L"animation\0*.anim\0ALL\0*.*";
	Desc.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST; // 이건 고정
	Desc.lpstrInitialDir = StrContentPath.c_str(); // 파일열었을시 경로를 설정해줌
	

	// 위에꺼랑 다른거는 열어주냐 저장하냐의 이 함수 차이뿐
	if (GetOpenFileName(&Desc))
	{
		if (wcslen(szFilePath) != 0)
		{
			// 총 파일 길이
			wstring fullPath = szFilePath;

			// 아틀라스 파일 주소 알아내는 load 함수 발췌
			FILE* pFile = nullptr;
			_wfopen_s(&pFile, fullPath.c_str(), L"r");

			wchar_t szRead[255] = {};

			if (!pFile)
			{
				// 파일 열기에 실패한 경우
				MessageBox(NULL, fullPath.c_str(), L"Error", MB_ICONERROR);
				return result; // 함수 종료
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

					//이게 내가 찾는거
					result.Path = szRead;

				}
			}
			fclose(pFile);
			//여기까지

			// 찾고자 하는 경로를 제외한 나머지를 제거 strContentPath 에는 텍스쳐\\ 까지의 경로가 들어있음
			size_t pos = fullPath.find(StrContentPath);

			if (pos != wstring::npos)
			{
				// 경로 새로 설정해줌 컨텐츠까지만 가게
				StrContentPath = CPathMgr::Get()->GetContentPath();
				// 'StrContentPath' 이후의 경로만 남기기
				fullPath = fullPath.substr(pos + StrContentPath.length());
			}

			CAnimator* Ani = m_AniObj->GetEditAnimator();
			Ani->LoadAnimation(fullPath);
			
			// fullPath에 저장된 경로에서 파일명만 추출
			wstring fileName = fullPath.substr(fullPath.find_last_of(L"\\") + 1);

			// 파일명에서 확장자인 .anim 제거
			size_t dotPos = fileName.find(L".anim");

			if (dotPos != wstring::npos)
			{
				fileName = fileName.substr(0, dotPos);
			}

			// 찾은 파일명을 토대로 키값을 입력해서 아까 로드한 파일의 애니메이션을 재생해줌
			Ani->Play(fileName,true);
			result.Name = fileName;
			return result;
		}
	}

	// 함수의 반환 타입이 wstring 이므로 모든 조건에 걸리지 않을 경우 기본값인 빈 문자열 반환
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
	//내가 만든 코드 프레임 추가 사용법 
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
	// 특정 프레임 찾아서 수정 방법
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
		IDX = iter->second->m_vecFrm.size(); // 해당 객체의 벡터 크기 반환 (0-based index)
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
			// 1. 현재 레벨에 접근해야한다 GetCurrentLevel은 레벨포인터로 주니까 에디터 레벨로 다운캐스팅을 해서 사용한다.
			CLevel* pCurLevel = CLevelMgr::Get()->GetCurrentLevel();
			CLevel_Editor* pEditorLevel = dynamic_cast<CLevel_Editor*>(pCurLevel);

			// 버튼 등에 입력한 정수 값을 가져오는 함수 
			// 윈도우와 아이디를 알려줌
			int Minx = GetDlgItemInt(hWnd, IDC_Size_Min_X, nullptr, true);
			int Miny = GetDlgItemInt(hWnd, IDC_Size_Min_Y, nullptr, true);
			int Maxx = GetDlgItemInt(hWnd, IDC_Size_Max_X, nullptr, true);
			int Maxy = GetDlgItemInt(hWnd, IDC_Size_Max_Y, nullptr, true);

			const int TILE_SIZE = 64;

			// 시작 타일 위치 계산 (올림)
			int startTileX = static_cast<int>(std::floor(Minx / TILE_SIZE));
			int startTileY = static_cast<int>(std::floor(Miny / TILE_SIZE));

			// 끝 타일 위치 계산 (올림)
			int endTileX = static_cast<int>(std::ceil(Maxx / TILE_SIZE));
			int endTileY = static_cast<int>(std::ceil(Maxy / TILE_SIZE));

			// 타일 개수 계산
			int tileCountX = endTileX - startTileX;
			int tileCountY = endTileY - startTileY;

			CTileMap* pTileMap = pEditorLevel->GetTileMap();
			pTileMap->SetRowCol(tileCountX, tileCountY);
			

			// 만약 에디터 레벨이 아닌곳에서 메뉴바가 호출 됐다면 설계 오류고  널 포인트가 들어있을테니 어썰트를 건다.
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

	// 플랫폼 데이터
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

	// 기존 오브젝트 삭제
	DeleteAllObjects();

	wchar_t szBuffer[256] = {};
	int platformCount = 0;

	// [PLATFORM_DATA] 태그 찾기
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

		// 각 플랫폼 데이터 읽기
		fwscanf_s(pFile, L"%s %d", szBuffer, (unsigned)_countof(szBuffer), &platformIndex);
		fwscanf_s(pFile, L"%s %d", szBuffer, (unsigned)_countof(szBuffer), &type);
		fwscanf_s(pFile, L"%s %f %f", szBuffer, (unsigned)_countof(szBuffer), &startPos.x, &startPos.y);
		fwscanf_s(pFile, L"%s %f %f", szBuffer, (unsigned)_countof(szBuffer), &scale.x, &scale.y);

		// 객체 생성 및 추가
		CObj* pObj = nullptr;
		LAYER_TYPE layerType;

		if (type == 1) // 플랫폼
		{
			pObj = new CPlatform;
			pObj->SetName(L"Platform");
			layerType = LAYER_TYPE::PLATFORM;
		}
		else if (type == 2) // 플로어
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
	// 경로 얻어주고
	wstring StrContentPath = CPathMgr::Get()->GetContentPath();
	StrContentPath += L"platform\\";
	wchar_t szFilePath[255] = {}; // 배열 초기화

	OPENFILENAME Desc = {}; // 파일 구조체 초기화
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
			// SaveMap 함수 호출
			SaveMap(szFilePath);
		}
	}

	CUIMgr::Get()->SetPreventFrame(1);
}

void CLevel_Editor::LoadPlatform()
{
	// 경로 얻어주고
	wstring StrContentPath = CPathMgr::Get()->GetContentPath();
	StrContentPath += L"platform\\";
	wchar_t szFilePath[255] = {}; // 배열 초기화

	OPENFILENAME Desc = {}; // 파일 구조체 초기화
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
			// LoadMap 함수 호출
			LoadMap(szFilePath);
		}
	}

	CUIMgr::Get()->SetPreventFrame(1);
}