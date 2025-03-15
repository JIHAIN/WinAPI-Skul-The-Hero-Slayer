#pragma once
#include "CBase.h"

#include "CBase.h"
#include "CStateMachine.h"
#include "CLogMgr.h"
#include "CLevelMgr.h"
#include "CLevel.h"
#include "CPlayer.h"
#include "CTimeMgr.h"
#include "CSound.h"
#include "CAssetMgr.h"

class CState :
    public CBase
{
private:
    CStateMachine* m_Owner; // 나를 소유한 스테이트 머신.

public:
    CStateMachine* GetStateMachine() { return m_Owner; }

  
    template<typename T>
    T* GetOwner()  /// 나를 소유한 오브젝트를 받아오는 함수
    {
        T* pObject = dynamic_cast<T*>(m_Owner->GetOwner());
        assert(pObject);
        return pObject;
    }

public:
    virtual void Enter() = 0;      // 처음에 한번 해야할일
    virtual void FinalTick() = 0;  // 계속 할일
    virtual void Exit() = 0;       // 나갈때 한번 해야하는 일

    // 애니메이션 전환을 여기서 하면 좋음

public:
    virtual CState* Clone() = 0;
    CState();
    ~CState();

    friend class CStateMachine;
};

