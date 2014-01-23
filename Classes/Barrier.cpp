//
//  Barrier.cpp
//  BlackWhiteRush
//
//  Created by 何遵祖 on 13-12-30.
//
//

#include "Barrier.h"

Barrier* Barrier::instance(const char *file){
    Barrier* barrier = new Barrier();
    if (barrier && barrier->inits(file)) {
        barrier->autorelease();
        return barrier;
    }
    CC_SAFE_DELETE(barrier);
    return NULL;
}

bool Barrier::inits(const char *file){
    CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(file);
    if (!this->initWithSpriteFrame(frame)) {
        return false;
    }
    
    _objType = Object_Block;
    _barrierType = Barrier_None;
    _speed = 150;
    _boxRect = CCSizeMake(0.5f, 0.5f);
    
    return true;
}

void Barrier::onCollisionEnter(GameObject *collision){
    
}

void Barrier::barrierAction(){
    if (_barrierType == Barrier_Gear) {//旋转
        float newAngle  = CC_RADIANS_TO_DEGREES((CC_DEGREES_TO_RADIANS(_pB2Body->GetAngle()) + 0.001f));
        _pB2Body->SetTransform(_pB2Body->GetPosition(), newAngle);
    }
}