#pragma once
#include "CLevel.h"
class CLevel_Stage02 :
    public CLevel
{
private:

public:
    virtual void Init() override;   // ���� �ʱ�ȭ
    virtual void Exit() override;   // ������ �����Ҷ� ����

    virtual void Tick() override;

public:
    CLevel_Stage02(const CLevel_Stage02& _Other) = delete; //��������� ����
    CLevel_Stage02();
    ~CLevel_Stage02();
};

