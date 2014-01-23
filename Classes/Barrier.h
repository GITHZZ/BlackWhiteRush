//
//  Barrier.h
//  BlackWhiteRush
//
//  Created by 何遵祖 on 13-12-30.
//
//

#ifndef __BlackWhiteRush__Barrier__
#define __BlackWhiteRush__Barrier__

#include "cocos2d.h"
#include "GameLogic.h"

class Barrier : public GameObject{
public:
    static Barrier* instance(const char* file);
    virtual bool inits(const char* file);
    virtual void onCollisionEnter(GameObject* collision);
    
    void barrierAction();
private:
    CC_SYNTHESIZE(BarrierType, _barrierType, BarrierType);
};

#endif /* defined(__BlackWhiteRush__Barrier__) */
