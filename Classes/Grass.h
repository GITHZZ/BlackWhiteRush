//
//  Grass.h
//  BlackWhiteRush
//
//  Created by 何遵祖 on 14-1-6.
//
//

#ifndef __BlackWhiteRush__Grass__
#define __BlackWhiteRush__Grass__

#include "cocos2d.h"
#include "GameObject.h"

class Grass : public GameObject{
public:
    static Grass* instance(const char* file);
    virtual bool inits(const char* file);
    virtual void onCollisionEnter(GameObject* collision);
};

#endif /* defined(__BlackWhiteRush__Grass__) */
