//
//  Land.h
//  BlackWhiteRush
//
//  Created by 何遵祖 on 14-1-10.
//
//

#ifndef __BlackWhiteRush__Land__
#define __BlackWhiteRush__Land__

#include "cocos2d.h"
#include "GameLogic.h"

class Land : public GameObject{
public:
    static Land* instance(const char* file);
    virtual bool inits(const char* file);
};

#endif /* defined(__BlackWhiteRush__Land__) */
