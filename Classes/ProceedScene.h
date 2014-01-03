//
//  ProceedScene.h
//  BlackWhiteRush
//
//  Created by 何遵祖 on 13-12-29.
//
//

#ifndef __BlackWhiteRush__ProceedScene__
#define __BlackWhiteRush__ProceedScene__

#include "cocos2d.h"
#include "GameLogic.h"

#define ViewZOrder 99
#define ControllerZOrder 100

using namespace cocos2d;

class ProceedScene : public CCScene{
public:
    CREATE_FUNC(ProceedScene);
    ~ProceedScene();
    bool init();
};

#endif /* defined(__BlackWhiteRush__ProceedScene__) */
