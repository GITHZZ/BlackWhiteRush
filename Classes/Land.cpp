//
//  Land.cpp
//  BlackWhiteRush
//
//  Created by 何遵祖 on 14-1-10.
//
//

#include "Land.h"

Land* Land::instance(const char *file){
    Land* land = new Land();
    if (land && land->inits(file)) {
        land->autorelease();
        return land;
    }
    CC_SAFE_DELETE(land);
    return NULL;
}

bool Land::inits(const char *file){
    CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(file);
    if (!this->initWithSpriteFrame(frame)) {
        return false;
    }
    
    _objType = Object_Land;
    _speed = 150;
    //_boxRect = CCSizeMake(0.5f, 0.5f);
    
    return  true;
}

void Land::onCollisionEnter(GameObject *collision){
    
}