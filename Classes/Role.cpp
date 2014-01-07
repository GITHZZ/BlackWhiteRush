//
//  Role.cpp
//  BlackWhiteRush
//
//  Created by 何遵祖 on 13-12-30.
//
//

#include "Role.h"

Role* Role::instance(const char* file){
    Role *role = new Role();
    if (role && role->inits(file)) {
        role->autorelease();
        return role;
    }
    CC_SAFE_DELETE(role);
    return NULL;
}

bool Role::inits(const char* file){
    if (!this->initWithFile(file)) {
        return false;
    }
    
    _objType = Object_Role;
    _speed = 100;
    
    return true;
}

void Role::onCollisionEnter(GameObject* collision){
    CCLOG("++===============Collision Enter======================++");
}
