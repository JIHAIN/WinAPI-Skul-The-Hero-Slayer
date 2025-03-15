#pragma once

class CTexture;
class CSound;

class CAssetMgr
{
	SINGLE(CAssetMgr);
private:
	map<wstring, CTexture*>		m_mapTex;
	map<wstring, CSound*>		m_mapSound;

public:
	void Init();

public:
	CTexture* LoadTexture(const wstring& _strKey, const wstring& _RelativePath);
	CTexture* LoadEditTexture(const wstring& _strKey, const wstring& _RelativePath); // 내가 에디터 전용으로 만든거  const wstring& _strKey,

	CTexture* FindTexture(const wstring& _strKey);

	CTexture* CreateTexture(const wstring& _strKey, UINT _Width, UINT _Height);

	CSound* LoadSound(const wstring& _strKey, const wstring& _RelativePath);
	CSound* FindSound(const wstring& _strKey);
};
