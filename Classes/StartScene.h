//
//  StartScene.h
//  BlackWhiteRush
//
//  Created by 何遵祖 on 14-1-23.
//
//

#ifndef __BlackWhiteRush__StartScene__
#define __BlackWhiteRush__StartScene__

#include "cocos2d.h"

USING_NS_CC;

class StartScene : public CCLayer{
public:
    CCScene* scene();
    CREATE_FUNC(StartScene);
    bool init();
};

#endif /* defined(__BlackWhiteRush__StartScene__) */
