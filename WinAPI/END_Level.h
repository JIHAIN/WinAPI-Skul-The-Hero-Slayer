#pragma once
#include "CLevel.h"
class END_Level :
    public CLevel
{
private:

public:
    virtual void Init() override;   // ���� �ʱ�ȭ
    virtual void Exit() override;   // ������ �����Ҷ� ����

    virtual void Tick() override;

public:
    END_Level(const END_Level& _Other) = delete; //��������� ����
    END_Level();
    ~END_Level();
};

