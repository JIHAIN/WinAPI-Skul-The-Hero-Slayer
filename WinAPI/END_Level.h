#pragma once
#include "CLevel.h"
class END_Level :
    public CLevel
{
private:

public:
    virtual void Init() override;   // 레벨 초기화
    virtual void Exit() override;   // 레벨을 종료할때 수행

    virtual void Tick() override;

public:
    END_Level(const END_Level& _Other) = delete; //복사생성자 삭제
    END_Level();
    ~END_Level();
};

