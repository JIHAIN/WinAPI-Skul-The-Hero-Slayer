#pragma once
#include "CLevel.h"
class CLevel_Stage01 :
    public CLevel
{
private:

public:
    virtual void Init() override;   // ���� �ʱ�ȭ
    virtual void Exit() override;   // ������ �����Ҷ� ����

    virtual void Tick() override;

public:
    CLevel_Stage01(const CLevel_Stage01& _Other) = delete; //��������� ����
    CLevel_Stage01();
    ~CLevel_Stage01();
};

