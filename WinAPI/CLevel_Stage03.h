#pragma once
#include "CLevel.h"
class CLevel_Stage03 :
    public CLevel
{
private:

public:
    virtual void Init() override;   // ���� �ʱ�ȭ
    virtual void Exit() override;   // ������ �����Ҷ� ����

    virtual void Tick() override;

public:
    CLevel_Stage03(const CLevel_Stage03& _Other) = delete; //��������� ����
    CLevel_Stage03();
    ~CLevel_Stage03();
};

