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
	: CObj(_Other) // �׻� �θ� ��������� ȣ�� ���ְ�
	, m_Parent(nullptr)
	, m_MouseOn(false)
	, m_MouseOn_Prev(false)
	, m_LbtnDown(false)
{

	// �ڽ� UI�鿡�� Ŭ���� ��û�ϰ� �װ� �ֵ� ���ϵ�� �־��� 
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
	// ���� ��ġ ���
	m_FinalPos = GetPos();

	if (m_Parent) // �θ� �ִٸ� �θ��� ��ǥ�� �����ͼ� �� �����ǥ�� ����ؼ� ������ǥ�� �Ի���
	{
		m_FinalPos += m_Parent->GetFinalPos();

	}
	
	// ���콺üũ
	MouseOnCheak();

	// UI Tick
	Tick_UI();  
	// �ڽĿ��� ƽ�� �������� �θ��ʿ��� �ؾ��� �� ���ϸ� 
	// ���⼺ ���� ���� ���� Ŭ������ �ڽ��� �������̵��� ���� Tick_UI �� ���� ������ ��
	// �� �ڽ� ���� Tick�� �����Լ� ���̺�� ���ִ°��� 

	// �ڽ� UI Tick
	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->Tick();
	}
}

void CUI::FinalTick()
{
	CObj::FinalTick();

	// �ڽ� UI Tick
	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->Tick();
	}


}

void CUI::Render()
{
	Render_UI(); // �� �Լ��� �����ϴ°ɷ� �ڱ��� ������ ��ĥ �� ����.

	// �ڽ� UI Render
	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->Render();
	}

}

void CUI::Render_UI() // �ڽ��� �̰� �������̵� ���� �ʴ´ٸ� �׳� �⺻ â �����ְ� �ϸ� �׿� �°� �����ش�.
{
	PEN_TYPE pen = PEN_TYPE::GREEN;

	if (m_MouseOn == true)
	{
		pen = PEN_TYPE::BLUE;
	}

	SELECT_PEN(BackDC, pen);
	SELECT_BRUSH(BackDC, BRUSH_TYPE::HOLLOW);

	// ��������� �ϸ� �ΰ�� ����
	// ���� �ڽĵ��� ���ܼ� ���̳� ������ ������ ������� ƽ���� ����ؼ� �ٰ���
	Vec2 vScale = GetScale();
	Rectangle(BackDC, (int)m_FinalPos.x, (int)m_FinalPos.y, (int)m_FinalPos.x + (int)vScale.x, (int)m_FinalPos.y + (int)vScale.y);

}

void CUI::MouseOnCheak() // �̰� ������ǥ�� ����ϴ� UI�� ����ϰ� ������ǥ�� �ʿ��ҽ� �������̵� �ؾ���.
{
	// ���콺�� UI ���� �ö���ִ��� Ȯ���Ѵ�. 
	m_MouseOn_Prev = m_MouseOn;

	Vec2 vMousePos = CKeyMgr::Get()->GetMousePos();
	// Vec2 vPos = GetPos(); �ڽ� ������ ���� �̰� �ƴ� ���̳���� �� 
	Vec2 vScale = GetScale();


	// ���콺�� UI���� �ִ��� üũ
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

