#pragma once

class CUI;

class CUIMgr
{
	SINGLE(CUIMgr)

private:
	int  m_PreventFrame;

public:
	void Tick();

public:
	void SetPreventFrame(int _FrameCount) //  지정한 프레임 만큼 UI동작 금지하게 하는 코드
	{
		m_PreventFrame = _FrameCount;
	}

private:
	CUI* GetPriorityUI(CUI* _ParentUI);
	void StatusCheck(CUI* _ParentUI);
};

// 하는 일은 UI에 무슨 일이 생겼을시 그 상태를 지정해주는 역활