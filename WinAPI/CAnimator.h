#pragma once
#include "CComponent.h"



struct tAnimDesc
{
    wstring     AnimName;
    CTexture* pAtlas;
    Vec2        StartLeftTop;
    Vec2        SliceSize;
    int         FrmCount;
    int         FPS;
};

class CAnim;
class CTexture;
struct tAnimFrm;


// 다수의 Animation 을 관리
class CAnimator :
    public CComponent
{
private:
    map<wstring, CAnim*>    m_mapAnim;
    CAnim* m_CurAnim;
    bool                    m_Repeat;

    CAnim* m_PrevAnim;
    CAnim* m_NextAnim;
    bool                    m_ReturnToPrevious;
    map<wstring, wstring>   m_TransitionMap;

public:
    void CreateAnimation(const tAnimDesc& _Info);
    // 내가 만든버전
    map<wstring, CAnim*>  GetMapAnimator() { return m_mapAnim; }
    void CreateAnimationEdit(const tAnimDesc& _Info);
    void AddFrameToAnimation(const wstring& animName, const tAnimFrm& frame);
    void SaveEditAnimation(const wstring& _RelativeFolder, wstring _Name);

    // 재생용 추가코드

    // 현재 애니메이션과 다음 애니메이션을 설정하는 함수
    void SetNextAnimation(const wstring& _CurName, const wstring& _NextName);
    void InterruptAnimation(const wstring& _Name, bool _ReturnToPrevious = true);
    void ResumeAnimation();
    // 애니메이션 상태를 업데이트하고 필요시 다음 애니메이션으로 전환하는 함수
    void UpdateAnimationState();


    CAnim* FindAnimation(const wstring& _Name);

    void DeleteAnimation(wstring _Name);

    void Play(const wstring& _Name, bool _Repeat);
    void Stop() { m_CurAnim = nullptr; }
    void Render();

    void UI_Render();

    void SaveAnimation(const wstring& _RelativeFolder);
    void LoadAnimation(const wstring& _RelativePath);

public:
    virtual void FinalTick() override;

public:
    CLONE(CAnimator);
    CAnimator();
    CAnimator(const CAnimator& _Other);
    ~CAnimator();
};

