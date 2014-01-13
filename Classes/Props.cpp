//
//  Props.cpp
//  BlackWhiteRush
//
//  Created by 何遵祖 on 13-12-30.
//
//

#include "Props.h"

Props* Props::instance(const char* file){
    Props* prop = new Props();
    if (prop && prop->inits(file)) {
        prop->autorelease();
        return prop;
    }
    CC_SAFE_DELETE(prop);
    return NULL;
}

bool Props::inits(const char* file){
    if (!this->initWithFile(file)) {
        return false;
    }
    
    _objType = Object_Prop;
    _speed = 100;
    _boxRect = CCSizeMake(0.5f, 0.5f);
    
    return true;
}

void Props::onCollisionEnter(GameObject* collision){
    
}