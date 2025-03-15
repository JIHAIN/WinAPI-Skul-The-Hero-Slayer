#pragma once
class CLogMgr
{
	SINGLE(CLogMgr);
private:
	list<tLog> m_LogList;    //���� �α�
	float      m_LogStep;    //�αװ� ����
	float	   m_LogMaxLife; //�α� ������Ÿ��

public:
	void SetLogMaxLife(float _Life) { m_LogMaxLife = _Life; }
	// �α� ���̸� Ǫ������ �ƴ϶� ����Ʈ�� ������ �׾��ִ� ������ ����.
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
