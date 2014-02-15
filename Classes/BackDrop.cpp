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
    CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(file);
    if (!this->initWithSpriteFrame(frame) ) {
        return false;
    }
    
    _objType = Object_Background;
    _speed = 180;
    _boxRect = CCSizeMake(0.0f, 0.0f);
    _groundIndex = -1;
    
    return true;
}