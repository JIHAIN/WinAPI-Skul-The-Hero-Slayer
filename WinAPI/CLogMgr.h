#pragma once
class CLogMgr
{
	SINGLE(CLogMgr);
private:
	list<tLog> m_LogList;    //쌓인 로그
	float      m_LogStep;    //로그간 간격
	float	   m_LogMaxLife; //로그 라이프타임

public:
	void SetLogMaxLife(float _Life) { m_LogMaxLife = _Life; }
	// 로그 쌓이면 푸쉬백이 아니라 프런트로 앞으로 쌓아주는 식으로 간다.
	void AddLog(const tLog& _Log) { m_LogList.push_front(_Log); } 
	void SetLogStep(float _Step) { m_LogStep = _Step; }

public:
	void Tick();
};

#define LOG(_Log,_str)		wstring FuncName;\
							string str(__FUNCTION__);\
							FuncName = wstring(str.begin(), str.end());\
							wchar_t szBuff[255] = {};\
							swprintf_s(szBuff, 255, L"FunctionName : %s, {Line : %d}, %s", FuncName.c_str(), __LINE__, _str);\
							CLogMgr::Get()->AddLog(tLog{ szBuff ,_Log , 0.f });

#define MOUSELOG(_Log,_str)	wstring FuncName;\
							wchar_t szBuff[255] = {};\
							swprintf_s(szBuff, 255, _str);\
							CLogMgr::Get()->AddLog(tLog{ szBuff ,_Log , 0.f });
