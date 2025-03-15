#include "pch.h"
#include "CPanel.h"

#include "CKeyMgr.h"
#include "CEngine.h"
#include "CLogMgr.h"

CPanel::CPanel()
	: m_GrabPos(Vec2(0.f ,0.f))
{
}

CPanel::~CPanel()
{
}


void CPanel::Tick_UI()
{
	if (IsLBtnDown())
	{
		Vec2 vCurMousePos = CKeyMgr::Get()->GetMousePos(); // 현재 마우스 좌표 받아서
		Vec2 vDiff = vCurMousePos - m_GrabPos;			   // 현재 좌표와 클릭이 들어온 좌표의 차이를 구하고

		Vec2 vPos = GetPos();                              // 내 위치를 받아서
		vPos += vDiff;                                     // 내 위치에 차이값 만큼 더해주고
		SetPos(vPos);									   // 위치 설정

		m_GrabPos = vCurMousePos;                          // 현재 마우스 좌표를 클릭이 들어온 좌표에 넣어줌
	}
}

void CPanel::Render_UI()
{
	PEN_TYPE pen = PEN_TYPE::GREEN;

	if (IsMouseOn())
	{
		pen = PEN_TYPE::BLUE;
	}

	SELECT_PEN(BackDC, pen);
	SELECT_BRUSH(BackDC, BRUSH_TYPE::SKYBLUE);

	// 랜더포즈로 하면 두고와 버림
	// 이제 자식들이 생겨서 파이널 포즈라는 변수를 만들었음 틱에서 계산해서 줄거임
	Vec2 m_FinalPos = GetFinalPos();
	Vec2 vScale = GetScale();
	Rectangle(BackDC, (int)m_FinalPos.x, (int)m_FinalPos.y, 
		(int)m_FinalPos.x + (int)vScale.x, (int)m_FinalPos.y + (int)vScale.y);

}

void CPanel::BeginHovered()
{
}



void CPanel::LBtnDown() // UI매니저가 이벤트 체크 다해줌
{
	// 마우스 클릭이 일어 났을시 마우스 좌표를 받아둠
	m_GrabPos = CKeyMgr::Get()->GetMousePos();
}

