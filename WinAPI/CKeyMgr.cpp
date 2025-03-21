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
	// 게임 윈도우가 포커싱중 일때만 동작한다.
	if (CEngine::Get()->GetMainHwnd() == GetFocus())
	{

		for (size_t i = 0; i < m_vecKey.size(); ++i)
		{
			// 키가 눌려있는지 확인
			if (GetAsyncKeyState(g_arrKeyValue[i]) & 0x8001)
			{
				// 키가 눌려있고, 이전에는 눌려있지 않았다.
				if (false == m_vecKey[i].PrevPressed)
				{
					m_vecKey[i].State = KEY_STATE::TAP;
				}

				// 키가 눌려있고, 이전에도 눌려있었다.
				else
				{
					m_vecKey[i].State = KEY_STATE::PRESSED;
				}

				m_vecKey[i].PrevPressed = true;
			}

			// 키가 눌려있지 않다면
			else
			{
				// 이전에는 눌려있었다.
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

	// 게임윈도우가 포커싱중이 아닐경우
	else
	{
		for (size_t i = 0; i < m_vecKey.size(); ++i)
		{
			// 탭이나 프레스 중에 포커싱이 해제된다면 릴리즈를 해주고
			if (KEY_STATE::TAP == m_vecKey[i].State || (KEY_STATE::PRESSED == m_vecKey[i].State))
			{
				m_vecKey[i].State = KEY_STATE::RELEASED;
			}
			//릴리즈 중이였던 건 NONE 상태로 변하게 해준다.
			else if (KEY_STATE::RELEASED == m_vecKey[i].State)
			{
				m_vecKey[i].State = KEY_STATE::NONE;
			}

			// 모든 키들의 이전 누름상태는 false로 한다.
			m_vecKey[i].PrevPressed = false;
		}
	}
}
