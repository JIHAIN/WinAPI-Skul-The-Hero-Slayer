#pragma once
#include "CLevel.h"
class CLevel_Stage04 :
    public CLevel
{
private:

public:
    virtual void Init() override;   // ���� �ʱ�ȭ
    virtual void Exit() override;   // ������ �����Ҷ� ����

    virtual void Tick() override;

public:
    CLevel_Stage04(const CLevel_Stage04& _Other) = delete; //��������� ����
    CLevel_Stage04();
    ~CLevel_Stage04();
};


