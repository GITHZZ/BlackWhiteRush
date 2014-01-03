//
//  Role.h
//  BlackWhiteRush
//
//  Created by 何遵祖 on 13-12-30.
//
//

#ifndef __BlackWhiteRush__Role__
#define __BlackWhiteRush__Role__

#include "cocos2d.h"
#include "GameObject.h"

using namespace cocos2d;

enum RoleState{
    Role_Move,
    Role_Death,
    Role_Jump
};

class Role : public GameObject{
public:
    static Role* instance(const char* file);
    virtual bool inits(const char* file);
    virtual void onCollisionEnter(GameObject* collision);
};

#endif /* defined(__BlackWhiteRush__Role__) */
