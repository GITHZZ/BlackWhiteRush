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
    if (this->initWithFile(file)) {
        return false;
    }
    
    _objType = Object_Block;
    
    return true;
}

void Barrier::onCollisionEnter(GameObject *collision){
    
}