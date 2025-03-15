#pragma once


class CLevel;

class CLevelMgr
{
	SINGLE(CLevelMgr);
private:
	CLevel*		m_arrLevel[(int)LEVEL_TYPE::END];	// 모든 레벨
	CLevel*		m_CurLevel; // 현재 지정된 레벨

public:
	CLevel* GetCurrentLevel() { return m_CurLevel ; }

public:
	void Init();
	void Tick();
	void Render();

private:
	void ChangeLevel(LEVEL_TYPE _Type);
	friend class CTaskMgr;
};

