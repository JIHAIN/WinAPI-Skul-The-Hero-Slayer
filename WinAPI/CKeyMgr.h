#pragma once

enum class KEY
{
	W,
	S,
	A,
	D,
	Q,
	E,
	Z,
	X,
	C,
	F,
	G,
	H,

	LEFT,
	RIGHT,
	UP,
	DOWN,

	SPACE,
	ENTER,
		
	NUM_1,
	NUM_2,
	NUM_3,
	NUM_4,
	NUM_5,
	NUM_6,
	NUM_7,
	NUM_8,
	NUM_9,
	NUM_0,

	VK_1,
	VK_2,
	VK_3,
	VK_4,
	VK_5,
	VK_6,
	VK_7,
	VK_8,
	VK_9,

	LBTN,
	RBTN,

	END,
};



enum class KEY_STATE
{
	TAP,		// 막 눌린 상태
	PRESSED,	// 계속 눌려있는 상태
	RELEASED,	// 막 뗀 상태
	NONE,		// 눌려있지 않은 상태
};


struct tKeyInfo
{
	KEY_STATE	State;
	bool		PrevPressed;
};


class CKeyMgr
{
	SINGLE(CKeyMgr);
private:
	vector<tKeyInfo>	m_vecKey;
	Vec2				m_MousePos;


public:
	void Init();
	void Tick();

	KEY_STATE GetKeyState(KEY _Key) { return m_vecKey[(int)_Key].State; }
	Vec2 GetMousePos() { return m_MousePos; }
};

