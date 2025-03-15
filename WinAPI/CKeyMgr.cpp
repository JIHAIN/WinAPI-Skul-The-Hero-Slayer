#include "pch.h"
#include "CKeyMgr.h"

#include "CEngine.h"

int g_arrKeyValue[(int)KEY::END]
=
{
	'W',
	'S',
	'A',
	'D',
	'Q',
	'E',
	'Z',
	'X',
	'C',
	'F',
	'G',
	'H',

	VK_LEFT,
	VK_RIGHT,
	VK_UP,
	VK_DOWN,

	VK_SPACE,
	VK_RETURN,

	VK_NUMPAD1,
	VK_NUMPAD2,
	VK_NUMPAD3,
	VK_NUMPAD4,
	VK_NUMPAD5,
	VK_NUMPAD6,
	VK_NUMPAD7,
	VK_NUMPAD8,
	VK_NUMPAD9,
	VK_NUMPAD0,

	0x31,
	0x32,
	0x33,
	0x34,
	0x35,
	0x36,
	0x37,
	0x38,
	0x39,

	VK_LBUTTON,
	VK_RBUTTON,
};




CKeyMgr::CKeyMgr()
{

}

CKeyMgr::~CKeyMgr()
{

}


void CKeyMgr::Init()
{
	for (int i = 0; i < (int)KEY::END; ++i)
	{
		m_vecKey.push_back(tKeyInfo{KEY_STATE::NONE, false});
	}
}

void CKeyMgr::Tick()
{
	// ���� �����찡 ��Ŀ���� �϶��� �����Ѵ�.
	if (CEngine::Get()->GetMainHwnd() == GetFocus())
	{

		for (size_t i = 0; i < m_vecKey.size(); ++i)
		{
			// Ű�� �����ִ��� Ȯ��
			if (GetAsyncKeyState(g_arrKeyValue[i]) & 0x8001)
			{
				// Ű�� �����ְ�, �������� �������� �ʾҴ�.
				if (false == m_vecKey[i].PrevPressed)
				{
					m_vecKey[i].State = KEY_STATE::TAP;
				}

				// Ű�� �����ְ�, �������� �����־���.
				else
				{
					m_vecKey[i].State = KEY_STATE::PRESSED;
				}

				m_vecKey[i].PrevPressed = true;
			}

			// Ű�� �������� �ʴٸ�
			else
			{
				// �������� �����־���.
				if (m_vecKey[i].PrevPressed)
				{
					m_vecKey[i].State = KEY_STATE::RELEASED;
				}
				else
				{
					m_vecKey[i].State = KEY_STATE::NONE;
				}

				m_vecKey[i].PrevPressed = false;
			}
		}

		POINT Pos;
		GetCursorPos(&Pos);
		ScreenToClient(CEngine::Get()->GetMainHwnd(), &Pos);
		m_MousePos = Pos;
	}

	// ���������찡 ��Ŀ������ �ƴҰ��
	else
	{
		for (size_t i = 0; i < m_vecKey.size(); ++i)
		{
			// ���̳� ������ �߿� ��Ŀ���� �����ȴٸ� ����� ���ְ�
			if (KEY_STATE::TAP == m_vecKey[i].State || (KEY_STATE::PRESSED == m_vecKey[i].State))
			{
				m_vecKey[i].State = KEY_STATE::RELEASED;
			}
			//������ ���̿��� �� NONE ���·� ���ϰ� ���ش�.
			else if (KEY_STATE::RELEASED == m_vecKey[i].State)
			{
				m_vecKey[i].State = KEY_STATE::NONE;
			}

			// ��� Ű���� ���� �������´� false�� �Ѵ�.
			m_vecKey[i].PrevPressed = false;
		}
	}
}
