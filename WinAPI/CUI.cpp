#include "pch.h"
#include "CUI.h"
#include "CEngine.h"
#include "CKeyMgr.h"



CUI::CUI()
	: m_LbtnDown(false)
	, m_MouseOn(false)
	, m_MouseOn_Prev(false)
	, m_FinalPos(0.f , 0.f)
	, m_Parent(nullptr)
{
}

CUI::CUI(const CUI& _Other)
	: CObj(_Other) // 항상 부모꺼 복사생성자 호출 해주고
	, m_Parent(nullptr)
	, m_MouseOn(false)
	, m_MouseOn_Prev(false)
	, m_LbtnDown(false)
{

	// 자식 UI들에게 클론을 요청하고 그걸 애드 차일드로 넣어줌 
	for (size_t i = 0; i < _Other.m_vecChild.size(); ++i)
	{
		AddChildUI(_Other.m_vecChild[i]->Clone());
	}

	
}

CUI::~CUI()
{
	Release_Vector(m_vecChild);
}

void CUI::Tick()
{
	// 최종 위치 계산
	m_FinalPos = GetPos();

	if (m_Parent) // 부모가 있다면 부모의 좌표를 가져와서 내 상대좌표랑 계산해서 최종좌표를 게산함
	{
		m_FinalPos += m_Parent->GetFinalPos();

	}
	
	// 마우스체크
	MouseOnCheak();

	// UI Tick
	Tick_UI();  
	// 자식에게 틱이 가기전에 부모쪽에서 해야할 일 다하면 
	// 다향성 으로 인해 실제 클래스인 자식이 오버라이딩한 본인 Tick_UI 에 적은 할일을 함
	// 즉 자식 역의 Tick을 가상함수 테이블로 해주는거임 

	// 자식 UI Tick
	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->Tick();
	}
}

void CUI::FinalTick()
{
	CObj::FinalTick();

	// 자식 UI Tick
	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->Tick();
	}


}

void CUI::Render()
{
	Render_UI(); // 이 함수를 조정하는걸로 자기의 랜더를 고칠 수 있음.

	// 자식 UI Render
	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->Render();
	}

}

void CUI::Render_UI() // 자식이 이걸 오버라이딩 하지 않는다면 그냥 기본 창 보여주고 하면 그에 맞게 보여준다.
{
	PEN_TYPE pen = PEN_TYPE::GREEN;

	if (m_MouseOn == true)
	{
		pen = PEN_TYPE::BLUE;
	}

	SELECT_PEN(BackDC, pen);
	SELECT_BRUSH(BackDC, BRUSH_TYPE::HOLLOW);

	// 랜더포즈로 하면 두고와 버림
	// 이제 자식들이 생겨서 파이널 포즈라는 변수를 만들었음 틱에서 계산해서 줄거임
	Vec2 vScale = GetScale();
	Rectangle(BackDC, (int)m_FinalPos.x, (int)m_FinalPos.y, (int)m_FinalPos.x + (int)vScale.x, (int)m_FinalPos.y + (int)vScale.y);

}

void CUI::MouseOnCheak() // 이건 고정좌표를 사용하는 UI가 사용하고 랜더좌표가 필요할시 오버라이드 해야함.
{
	// 마우스가 UI 위에 올라와있는지 확인한다. 
	m_MouseOn_Prev = m_MouseOn;

	Vec2 vMousePos = CKeyMgr::Get()->GetMousePos();
	// Vec2 vPos = GetPos(); 자식 구조에 따라 이게 아닌 파이널포즈를 씀 
	Vec2 vScale = GetScale();


	// 마우스가 UI위에 있는지 체크
	if (m_FinalPos.x <= vMousePos.x && vMousePos.x <= m_FinalPos.x + vScale.x
		&& m_FinalPos.y <= vMousePos.y && vMousePos.y <= m_FinalPos.y + vScale.y)
	{
		m_MouseOn = true;
	}
	else
	{
		m_MouseOn = false;
	}

}

