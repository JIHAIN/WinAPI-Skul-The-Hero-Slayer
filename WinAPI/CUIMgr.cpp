#include "pch.h"
#include "CUIMgr.h"
#include "CLevelMgr.h"
#include "CLevel.h"
#include "CUI.h"
#include "CKeyMgr.h"

CUIMgr::CUIMgr()
	: m_PreventFrame(0)
{
}

CUIMgr::~CUIMgr()
{
}

void CUIMgr::Tick()
{
	if (m_PreventFrame)
	{
		--m_PreventFrame;
		return;
	}
	// ���콺 ���ʹ�ư�� ���¸� üũ�Ѵ�.
	KEY_STATE LBtnState = CKeyMgr::Get()->GetKeyState(KEY::LBTN);

	//���� ������ Ȯ��
	CLevel* pCurLevel = CLevelMgr::Get()->GetCurrentLevel();

	// ���� ���Ϳ� ��� ��� UI ���̾ �����´�.

	// ������  ������ ���Ϸ��� const & �� �޾ƿԴµ� ���� ������ �ʿ��ؼ� const�� ���ش�.
	//const vector<CObj*>& vecUI = pCurLevel->GetLayer(LAYER_TYPE::UI);
	vector<CObj*>& vecUI = pCurLevel->GetLayer(LAYER_TYPE::UI);

	vector<CObj*>::reverse_iterator iter = vecUI.rbegin();

	// ������ ���͸� ����ؼ� �ڿ��� ���� ������ ���������� �˻��ذ���.
	// ���⼭ �ɸ� �갡 ��Ŀ���� �޾ƾ� �ϴϱ� ������ �� �ڷ� ������ ������ �� �ڷ� ������.
	// �׸��� ���̻� �ݺ����� �� �ʿ䵵 ������
	for (; iter !=  vecUI.rend(); ++iter)
	{
		// Ȥ�ó��� ���� ����ó��
		CUI* pUI = dynamic_cast<CUI*>((*iter));
		assert(pUI);

		// �θ� UI�� �Է��ؼ� ���� �̺�Ʈ�� ������ �켱���� UI�� �˾Ƴ�
		// ������ ���� ���� �ְ� �ڽ��� ���� ���� �ִ�.
		CUI* pPriorityUI = GetPriorityUI(pUI);

		if (nullptr == pPriorityUI) // ���콺�� ���� �ö�� UI�� ������ ���� �ֻ��� UI�� �Ѿ��.
		{
			StatusCheck(pUI); // ���� üũ ���� �������߿� �������� �� ����� ���� ������ ����
			continue;
		}
			

		// pPriorityUI �� ���콺 �̺�Ʈ�� ���������� UI�� ���ϴ� ����
		
		if (LBtnState == KEY_STATE::TAP) // ��������
		{
			pPriorityUI->m_LbtnDown = true;
			pPriorityUI->LBtnDown();

			// ���Ϳ��� �� ���� Ǫ�������� �� �ڿ� ����ִ´�.
			vecUI.erase((iter + 1).base());
			vecUI.push_back(pUI);

			break;
		}

		if (LBtnState == KEY_STATE::RELEASED)
		{
			//pUI->m_LbtnDown = false; �̰� ���⼭ ȣ���ϸ� �� UI�������� �������� üũ�� ������ ���⼭ üũ����
			
			pPriorityUI->LBtnUP(); // UI������ �� UP�� ������
			if (pPriorityUI->m_LbtnDown) // �ش� UI ������ ���Ȱ� ���� ���� ���¶�� Ŭ������
			{
				pPriorityUI->LBtnClicked();
			}
			StatusCheck(pUI);
		}

		StatusCheck(pUI); // ����üũ ��� Ű ��ȸ ���鼭 LBtnDown�� false�� �������
		break;
	}
	
}

CUI* CUIMgr::GetPriorityUI(CUI* _ParentUI)
{
	// ���콺 ���ʹ�ư�� ���¸� üũ�Ѵ�.
	KEY_STATE LBtnState = CKeyMgr::Get()->GetKeyState(KEY::LBTN);

	// ��ȯ ������ �� �켱����ui ������ 
	CUI* pPriorityUI = nullptr; 

	// ������ȸ
	static list<CUI*> queue;
	queue.clear(); // �������� ������ �صּ� ��� �����Ͱ� ���������� �Ź� Ŭ��� ���ش�.

	// �ֻ��� �θ� UI�Է�
	queue.push_back(_ParentUI); 

	while (!queue.empty()) // ť�� ������� ������
	{
		CUI* pUI = queue.front(); // �տ� �ִ°� ������
		queue.pop_front(); // ���� �����


		// �� ���� ���콺�� �ִ��� ������ üũ
		if (pUI->m_MouseOn) // �̹� ���� UI�� �� Tick���� üũ �ص���
		{
			pPriorityUI = pUI; // ���� ������ �ڽ��� ���콺�� ���� �ִ� UI�� �̺�Ʈ�� ���������Ѵ�.
		}

		

		// ���콺 On ���� �̺�Ʈ
		if (false == pUI->m_MouseOn_Prev && pUI->m_MouseOn) // �̹� �����ӿ� ó�� ���콺�� �ö���� ��
			pUI->BeginHovered();
		else if (pUI->m_MouseOn_Prev && pUI->m_MouseOn) // ���콺�� ��� ��������.
			pUI->OnHovered();
		else if (pUI->m_MouseOn_Prev && !pUI->m_MouseOn) // ���콺 ���.
			pUI->EndHovered();


		const vector<CUI*>& vecChild = pUI->GetChildUI(); // �Է����� ���� UI�� �ڽ��� ������ ����.
		for (size_t i = 0; i < vecChild.size(); ++i)
		{
			queue.push_back(vecChild[i]);
		}
	}
	// UI�� �ִ� ��� �ڽİ��踦 �� ��ȸ�ؼ� ã�Ƴ� 
	return pPriorityUI;
}

void CUIMgr::StatusCheck(CUI* _ParentUI)
{
	// ���콺 ���ʹ�ư�� ���¸� üũ�Ѵ�.
	KEY_STATE LBtnState = CKeyMgr::Get()->GetKeyState(KEY::LBTN);

	// ������ȸ
	static list<CUI*> queue;
	queue.clear(); // �������� ������ �صּ� ��� �����Ͱ� ���������� �Ź� Ŭ��� ���ش�.

	// �ֻ��� �θ� UI�Է�
	queue.push_back(_ParentUI);

	while (!queue.empty()) // ť�� ������� ������
	{
		CUI* pUI = queue.front(); // �տ� �ִ°� ������
		queue.pop_front(); // ���� �����


		const vector<CUI*>& vecChild = pUI->GetChildUI(); // �Է����� ���� UI�� �ڽ��� ������ ����.
		for (size_t i = 0; i < vecChild.size(); ++i)
		{
			queue.push_back(vecChild[i]);
		}


		// ���� ��ư ���� ���� UI�ۿ��� ������ �������, ��� UI���� �������¸� false�� �ٲ�
		if (LBtnState == KEY_STATE::RELEASED)
		{
			pUI->m_LbtnDown = false;
		}
	}
	// UI�� �ִ� ��� �ڽİ��踦 �� ��ȸ�ؼ� ã�Ƴ� 
}
