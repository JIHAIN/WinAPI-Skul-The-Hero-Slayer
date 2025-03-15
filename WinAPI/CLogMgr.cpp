#include "pch.h"
#include "CLogMgr.h"
#include "CEngine.h"

#include "CTimeMgr.h"


CLogMgr::CLogMgr()
	:m_LogMaxLife(3.f)
	,m_LogStep(18.f)
{
}

CLogMgr::~CLogMgr()
{
}





void CLogMgr::Tick()
{
	list<tLog>::iterator iter = m_LogList.begin();

	int i = 0;

	// 반복문 돌면서 로그를 처리함 
	for (iter; iter != m_LogList.end();)
	{
		++i;
		// 로그를 화면에 출력시킨다.
		Vec2 vLogPos = Vec2(10.f, 10.f + (float)(i * m_LogStep));

		// 로그 수준에 따라 색상을 다르게 한다.
		
		switch ((*iter).Level)
		{
			
		case LOG_LEVEL::LOG:
			SetTextColor(BackDC, RGB(220, 220, 220));
			break;

		case LOG_LEVEL::WARNING:
			SetTextColor(BackDC, RGB(220, 220, 30));
			break;
			

		case LOG_LEVEL::BUG:
			SetTextColor(BackDC, RGB(220, 30, 30));
			break;
			
		}
		// 테두리 색 없애는 코드
		SetBkMode(BackDC, TRANSPARENT);
		TextOut(BackDC, (int)vLogPos.x, (int)vLogPos.y, (*iter).strLog.c_str(), (*iter).strLog.length());
		

		// 로그의 수명이 다 끝났으면 리스트에서 제거한다.
		(*iter).Time += DT;
		if (m_LogMaxLife < (*iter).Time)
		{
			iter = m_LogList.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}
