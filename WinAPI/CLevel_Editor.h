#pragma once
#include "CLevel.h"

class CTileMap;
class CAnimator;
class CTexture;
class CEditTextObj;
class CEditAniObj;
struct AniNamePath;

class CLevel_Editor
	: public CLevel
{
private:
	HWND		hDialog;     //AniProc 조작을 위한 핸들값 받아놓기
	HWND		hDialog2;    //MapProc 조작을 위한 핸들값
	HMENU		m_hMenu;
	CTileMap*   m_TileMap;
	CEditTextObj*   m_EditObj;// 텍스트용 테스트
	CEditAniObj*	m_AniObj; // 애니메이션용 테스트

	CAnimator*  m_Animator;
	CTexture*   m_Texture;

	// 맵 에디터용
	vector<tEditMapInfo> m_vecEditMap;

	TCHAR		FileName;
	bool        QON; 
	bool        EON;
	Vec2		vMousePos;			//내 마우스좌표
	Vec2		RealMousePos;		//실제 마우스 좌표
	Vec2		vMouseTapPos;		//탭 했을 떄 좌표
	Vec2		vMouseRealTapPos;
	Vec2		vMouseReleasedPos;  //릴리즈때 좌표
	Vec2		vMouseRealReleasedPos;

public:
	Vec2 GetRealTapPos() { return vMouseRealTapPos; }
	Vec2 GetRealReleasedPos() { return vMouseRealReleasedPos; }
	CEditAniObj* GetEditObj() { return m_AniObj; }
	void SetFileName(TCHAR _FileName) { FileName = _FileName; }
	void SetAnihWnd(HWND _hDialog) { hDialog = _hDialog; }
	void SetAnihWnd2(HWND _hDialog) { hDialog2 = _hDialog; }
	HWND GetAnihWnd() { return hDialog; }
	HWND GetAnihWnd2() { return hDialog2; }


public:
	CTileMap* GetTileMap() { return m_TileMap; }
	void SaveTile();
	void LoadTile();



	// 애니메이션 에디터용
	wstring  CreatAni(const wstring& _Key,const wstring& _Path, const wstring& _Nmae, int _FPS, float _Time, Vec2 _Slice, Vec2 _Left);
	wstring  SelectAtlas();
	AniNamePath LoadAni(); // 만든 구조체 wstring 2개가 있는 타입 불러온 파일의 이름과 경로를 반환해줌

	void AniMouseMode();
	void SaveAni(wstring _Path, wstring _Name);
	void EditPlay(const wstring _Name);
	void EditStop();
	void FrmAdd(Vec2 _LeftTop, Vec2 _Offset, Vec2 _Slice, float _Duration, const wstring& _animName);
	void FrmSave(Vec2 _LeftTop, Vec2 _Offset, Vec2 _Slice, float _Duration, const wstring& _animName, int _idx);
	void DurationSave(const wstring& _animName, float _Duration);

	// 맵 에디터용
	void SaveMap(const wstring& _FolderPath);
	void LoadMap(const wstring& _FilePath);

	void SavePlatform();

	void LoadPlatform();

private:
	void TileMode();

private:
	virtual void Init() override;
	virtual void Exit() override;
	virtual void Tick() override;

public:
	CLevel_Editor();
	~CLevel_Editor();
};
