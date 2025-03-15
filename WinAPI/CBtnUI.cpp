#include "pch.h"
#include "CBtnUI.h"

#include "Resource.h"
#include "CEngine.h"
#include "CLogMgr.h"

CBtnUI::CBtnUI()
	: m_CallBack(nullptr)
	, m_Inst(nullptr)
	, m_MemFunc(nullptr)
	, m_MemFunc_1(nullptr)
{
}

CBtnUI::~CBtnUI()
{
}

// 타일 프로세스 인식을 위한 전방선언
LRESULT CALLBACK TileProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);




void CBtnUI::LBtnDown()
{
}

void CBtnUI::LBtnClicked()
{
	if (m_CallBack)
	{
		// 등록된 함수 호출
		m_CallBack(); 
		LOG(LOG_LEVEL::WARNING, L"콜백 함수");
	}
	
	if (m_Inst && m_MemFunc) // 객체도 받았고 함수도 받았으면
	{
		// 멤버 객체에 접근해야해서 *포인터가 필요하다 이건 인자가 보이드여서 이렇게 씀
		(m_Inst->*m_MemFunc)(); 
		//만약 인자가 인트를 받는다면
		// (m_Inst->*m_MemFunc)(53); 이런식으로 인자 넣어줘야함.

		LOG(LOG_LEVEL::WARNING, L"델리게이트 함수");
	}

}

void CBtnUI::Render_UI()
{
	PEN_TYPE Pen = PEN_TYPE::GREEN;
	if (IsMouseOn())
	{
		Pen = PEN_TYPE::BLUE;
	}

	if (IsLBtnDown())
	{
		Pen = PEN_TYPE::RED;
	}

	SELECT_PEN(BackDC, Pen);
	SELECT_BRUSH(BackDC, BRUSH_TYPE::GRAY);

	Vec2 vFinalPos = GetFinalPos();
	Vec2 vScale = GetScale();
	Rectangle(BackDC, (int)vFinalPos.x, (int)vFinalPos.y
		, (int)vFinalPos.x + (int)vScale.x, (int)vFinalPos.y + (int)vScale.y);
}
