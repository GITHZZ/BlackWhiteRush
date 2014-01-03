//
//  backDrop.h
//  BlackWhiteRush
//
//  Created by 何遵祖 on 13-12-30.
//
//

#ifndef __BlackWhiteRush__backDrop__
#define __BlackWhiteRush__backDrop__

#include "cocos2d.h"
#include "GameObject.h"

class BackDrop : public GameObject{
public:
    static BackDrop* instance(const char* file);
    virtual bool inits(const char* file);
    virtual void onCollisionEnter(GameObject* collision);
};
#endif /* defined(__BlackWhiteRush__backDrop__) */
