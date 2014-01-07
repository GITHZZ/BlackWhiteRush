//
//  Grass.cpp
//  BlackWhiteRush
//
//  Created by 何遵祖 on 14-1-6.
//
//

#include "Grass.h"

Grass* Grass::instance(const char* file){
    Grass* grass = new Grass();
    if (grass && grass->inits(file)) {
        grass->autorelease();
        return grass;
    }
    CC_SAFE_DELETE(grass);
    return NULL;
}

bool Grass::inits(const char* file){
    CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(file);
    if (!this->initWithSpriteFrame(frame)) {
        return false;
    }
    
    _objType = Object_Grass;
    _speed = 150;
    
    return true;
}

void Grass::onCollisionEnter(GameObject *collision){
    
}