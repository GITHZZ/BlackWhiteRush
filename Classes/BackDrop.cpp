//
//  backDrop.cpp
//  BlackWhiteRush
//
//  Created by 何遵祖 on 13-12-30.
//
//

#include "backDrop.h"

BackDrop* BackDrop::instance(const char *file){
    BackDrop* backdrop = new BackDrop();
    if (backdrop && backdrop->inits(file)) {
        backdrop->autorelease();
        return backdrop;
    }
    CC_SAFE_DELETE(backdrop);
    return NULL;
}

bool BackDrop::inits(const char *file){
    if (!this->initWithFile(file)) {
        return false;
    }
    
    _objType = Object_Background;
    
    return true;
}

void BackDrop::onCollisionEnter(GameObject *collision){
    
}