#pragma once
#include "CLevel.h"
class CLevel_Stage05 :
    public CLevel
{
private:

public:
    virtual void Init() override;   // ���� �ʱ�ȭ
    virtual void Exit() override;   // ������ �����Ҷ� ����

    virtual void Tick() override;

public:
    CLevel_Stage05(const CLevel_Stage05& _Other) = delete; //��������� ����
    CLevel_Stage05();
    ~CLevel_Stage05();
};


