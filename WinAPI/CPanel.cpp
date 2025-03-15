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
		Vec2 vCurMousePos = CKeyMgr::Get()->GetMousePos(); // ���� ���콺 ��ǥ �޾Ƽ�
		Vec2 vDiff = vCurMousePos - m_GrabPos;			   // ���� ��ǥ�� Ŭ���� ���� ��ǥ�� ���̸� ���ϰ�

		Vec2 vPos = GetPos();                              // �� ��ġ�� �޾Ƽ�
		vPos += vDiff;                                     // �� ��ġ�� ���̰� ��ŭ �����ְ�
		SetPos(vPos);									   // ��ġ ����

		m_GrabPos = vCurMousePos;                          // ���� ���콺 ��ǥ�� Ŭ���� ���� ��ǥ�� �־���
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

	// ��������� �ϸ� �ΰ�� ����
	// ���� �ڽĵ��� ���ܼ� ���̳� ������ ������ ������� ƽ���� ����ؼ� �ٰ���
	Vec2 m_FinalPos = GetFinalPos();
	Vec2 vScale = GetScale();
	Rectangle(BackDC, (int)m_FinalPos.x, (int)m_FinalPos.y, 
		(int)m_FinalPos.x + (int)vScale.x, (int)m_FinalPos.y + (int)vScale.y);

}

void CPanel::BeginHovered()
{
}



void CPanel::LBtnDown() // UI�Ŵ����� �̺�Ʈ üũ ������
{
	// ���콺 Ŭ���� �Ͼ� ������ ���콺 ��ǥ�� �޾Ƶ�
	m_GrabPos = CKeyMgr::Get()->GetMousePos();
}

