#pragma once
#include "CBase.h"
#include "CAnimator.h"

struct tAnimFrm
{
    Vec2    vLeftTop;
    Vec2    vSlice;
    Vec2    vOffset;
    float   Duration;
};

class CTexture;

class CAnim :
    public CBase
{
private:
    CAnimator* m_Owner;

    CTexture* m_Atlas;
    int                 m_CurIdx;
    float               m_AccTime;
    bool                m_Finish;

    // 새 코드
    bool                m_IsPlaying;

public:
    vector<tAnimFrm>    m_vecFrm;

    void Create(const tAnimDesc& _Info);
    //내가 만든버전
    void CreateEditPlus(const tAnimDesc& _Info);
    void AddFrame(const tAnimFrm& _Frm) { m_vecFrm.push_back(_Frm); }
    void UI_Render();

    // 재생용 새 코드

    // 애니메이션이 현재 재생 중인지 확인
    bool IsPlaying() const { return m_IsPlaying; }
    // 애니메이션 재생 시작
    void Play() { m_IsPlaying = true; }
    // 애니메이션 재생 중지
    void Stop() { m_IsPlaying = false; }

    bool IsFinish() { return m_Finish; }
    void Reset()
    {
        m_Finish = false;
        m_AccTime = 0.f;
        m_CurIdx = 0;
    }

    tAnimFrm& GetFrame(int _Idx) { return m_vecFrm[_Idx]; }

public:
    void FinalTick();
    void Render();

    void Save(const wstring& _FolderPath);
    void Load(const wstring& _FilePath);



public:
    CLONE(CAnim);
    CAnim();
    CAnim(const CAnim& _Other);
    ~CAnim();

    friend class CAnimator;
};

