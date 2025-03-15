#pragma once
#include "CLevel.h"
class CLevel_Stage03 :
    public CLevel
{
private:

public:
    virtual void Init() override;   // 레벨 초기화
    virtual void Exit() override;   // 레벨을 종료할때 수행

    virtual void Tick() override;

public:
    CLevel_Stage03(const CLevel_Stage03& _Other) = delete; //복사생성자 삭제
    CLevel_Stage03();
    ~CLevel_Stage03();
};

