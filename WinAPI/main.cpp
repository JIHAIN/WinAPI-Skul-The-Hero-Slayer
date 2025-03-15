#include "pch.h"
#include "Resource.h"
#include "CEngine.h"



// 프로세스 ID
HINSTANCE g_hInst = nullptr;


// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

// hInstance : 프로세스 메모리 주소 or 프로세스 ID
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    // 메모리 누수 체크 On
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    //_CrtSetBreakAlloc(1308);

    // 생성시킬 윈도우의 세팅값 설정
    MyRegisterClass(hInstance);


    // 등록된 윈도우 클래스 정보로 윈도우 생성
    g_hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

    HWND hWnd = CreateWindowW(L"키값", L"My Game", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
        return FALSE;
    
    ShowWindow(hWnd, true);
    UpdateWindow(hWnd);

    // Engine 초기화
    CEngine::Get()->Init(hWnd, 1280, 800);

    // GetMessage 함수는 메세지큐에 메세지가 있으면 메세지를 꺼내온다.
    // 메세지큐에 메세지가 없으면 반환되지 않는다.
    // GetMessage 함수가 리턴됐다는 말은 메세지가 존재했다는 뜻이고,
    // GetMessage 리턴값은, WM_QUIT 메세지가 발생한 경우 0, 그 외에 메세지인 경우 1 이다.

    // PeekMessage
    // 메세지가 메세지큐에 있던 없던 반환된다.
    // 메세지큐에 메세지가 있으면 true,
    // 메세지큐에 메세지가 없으면 false,
    MSG msg = {};

    while (true)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (WM_QUIT == msg.message)
                break;

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        else
        {
            // 게임 구현
            // Progress 함수 1회 실행 == 1 프레임
            CEngine::Get()->Progress();
        }
    }

    return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex = {};

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINAPI));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = nullptr;//MAKEINTRESOURCEW(IDC_WINAPI);
    wcex.lpszClassName  = L"키값";
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    HRESULT result = RegisterClassExW(&wcex);

    return (ATOM)result;
}

//전방선언 실제구현은 이걸 사용할 곳에서 할거임
INT_PTR CALLBACK TileProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK AniProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK MapProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

#include "CLevel.h"
#include "CLevelMgr.h"
#include "CLevel_Editor.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

    HWND hDialog = nullptr; //모달리스를 위한 핸들 여기에 CreateDialog 해서 넣어줌.
    HWND hDialog2 = nullptr;

    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
            switch (wmId)
            {
            //case IDM_ABOUT: 내가 기존 아이디 박스 없애버림
            //    DialogBox(g_hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            //    break;

            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;

            case IDM_RowCol:
                DialogBox(g_hInst, MAKEINTRESOURCE(IDD_TILE_ROWCOL), hWnd, TileProc);
                break;

            case ID_TILE_SAVE:
            {
                CLevel_Editor* pLevel = dynamic_cast<CLevel_Editor*>(CLevelMgr::Get()->GetCurrentLevel());
                assert(pLevel);
                pLevel->SaveTile();
            }
            break;

            case ID_TILE_LOAD:
            {
                CLevel_Editor* pLevel = dynamic_cast<CLevel_Editor*>(CLevelMgr::Get()->GetCurrentLevel());
                assert(pLevel);
                pLevel->LoadTile();
            }
            break;
           
            // 내가 만든 애니메이션 용
            case ID_ANI_SET: // 오브젝트 레이어를 선택하고 그걸 수정하게 함
            {
                //DialogBox(g_hInst, MAKEINTRESOURCE(IDD_Anim), hWnd, AniProc); 이건 모달방식
                // 아래는 모달리스 방식

               hDialog = CreateDialog(g_hInst, MAKEINTRESOURCE(IDD_Anim), hWnd, AniProc);
               ShowWindow(hDialog, SW_SHOW);
               SetWindowPos(hDialog, nullptr, 950, 360, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

                CLevel_Editor* pLevel = dynamic_cast<CLevel_Editor*>(CLevelMgr::Get()->GetCurrentLevel());
                pLevel->SetAnihWnd(hDialog);
 
            }
                break;

            // 맵 에디터 만들기
            case ID_Map_Set:
            {
                hDialog2 = CreateDialog(g_hInst, MAKEINTRESOURCE(IDD_MAP), hWnd, MapProc);
                ShowWindow(hDialog2, SW_SHOW);
                SetWindowPos(hDialog2, nullptr, 950, 360, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

                CLevel_Editor* pLevel = dynamic_cast<CLevel_Editor*>(CLevelMgr::Get()->GetCurrentLevel());
                pLevel->SetAnihWnd2(hDialog2);
            }


            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;




    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
            EndPaint(hWnd, &ps);
        }
        break;

    case WM_KEYDOWN:
    {
        if (wParam == 'W')
            int a = 0;
    }     
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
