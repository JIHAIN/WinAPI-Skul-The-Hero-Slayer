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
	HWND		hDialog;     //AniProc ������ ���� �ڵ鰪 �޾Ƴ���
	HWND		hDialog2;    //MapProc ������ ���� �ڵ鰪
	HMENU		m_hMenu;
	CTileMap*   m_TileMap;
	CEditTextObj*   m_EditObj;// �ؽ�Ʈ�� �׽�Ʈ
	CEditAniObj*	m_AniObj; // �ִϸ��̼ǿ� �׽�Ʈ

	CAnimator*  m_Animator;
	CTexture*   m_Texture;

	// �� �����Ϳ�
	vector<tEditMapInfo> m_vecEditMap;

	TCHAR		FileName;
	bool        QON; 
	bool        EON;
	Vec2		vMousePos;			//�� ���콺��ǥ
	Vec2		RealMousePos;		//���� ���콺 ��ǥ
	Vec2		vMouseTapPos;		//�� ���� �� ��ǥ
	Vec2		vMouseRealTapPos;
	Vec2		vMouseReleasedPos;  //����� ��ǥ
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



	// �ִϸ��̼� �����Ϳ�
	wstring  CreatAni(const wstring& _Key,const wstring& _Path, const wstring& _Nmae, int _FPS, float _Time, Vec2 _Slice, Vec2 _Left);
	wstring  SelectAtlas();
	AniNamePath LoadAni(); // ���� ����ü wstring 2���� �ִ� Ÿ�� �ҷ��� ������ �̸��� ��θ� ��ȯ����

	void AniMouseMode();
	void SaveAni(wstring _Path, wstring _Name);
	void EditPlay(const wstring _Name);
	void EditStop();
	void FrmAdd(Vec2 _LeftTop, Vec2 _Offset, Vec2 _Slice, float _Duration, const wstring& _animName);
	void FrmSave(Vec2 _LeftTop, Vec2 _Offset, Vec2 _Slice, float _Duration, const wstring& _animName, int _idx);
	void DurationSave(const wstring& _animName, float _Duration);

	// �� �����Ϳ�
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
