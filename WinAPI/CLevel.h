#pragma once
#include "CBase.h"

class CObj;
class CPlayer;
class Boss_Monster;

struct PlatformData
{
    int type;
    Vec2 position;
    Vec2 scale;
};

// ��� ������ �θ�Ȱ

class CLevel :
    public CBase
{
private:    
    vector<CObj*>   m_arrLayer[(int)LAYER_TYPE::END];
    CPlayer*        m_CurPlayer;
    Boss_Monster* m_CurBoss;

public:
    void AddObject(CObj* _Obj, LAYER_TYPE _Type);
    //const vector<CObj*>& GetLayer(LAYER_TYPE _Type) { return m_arrLayer[(int)_Type]; }
    vector<CObj*>& GetLayer(LAYER_TYPE _Type) { return m_arrLayer[(int)_Type]; } //�����Ҽ� �ְ� const���� ����

    void RegisterPlayer(CPlayer* _Player) { m_CurPlayer = _Player; }

    void RegisterBoss(Boss_Monster* _Boss) { m_CurBoss = _Boss; }

    CPlayer* GetPlayer() { return m_CurPlayer; }
    Boss_Monster* GetBoss() { return m_CurBoss; }

    void DeleteObjects(LAYER_TYPE _Type);
    void DeleteAllObjects();

    // �� ����
    vector<PlatformData> m_PlatformData;

    void LoadPlatformData(const wstring& filePath);
    void CreatePlatformsFromData();

public:
    virtual void Init() = 0;
    virtual void Exit() = 0;


    virtual void Tick();
    virtual void FinalTick();
    virtual void Render();

public:
    CLONE_DISABLE(CLevel);
    CLevel();
    virtual ~CLevel();
};

