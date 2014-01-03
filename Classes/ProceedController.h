//
//  ProceedController.h
//  BlackWhiteRush
//
//  Created by 何遵祖 on 13-12-29.
//
//

/*
    游戏控制部分,同时也做渲染工作,不过主要是渲染游戏的控制部分,还有就是按钮(控制,FEVER,暂停)
    
    渲染工作
 
    用户可以控制部分,可以改变游戏逻辑部分,逻辑部分不影响它
 */

#ifndef __BlackWhiteRush__ProceedController__
#define __BlackWhiteRush__ProceedController__

#include "cocos2d.h"
#include "GameLogic.h"

using namespace cocos2d;

class ProceedController : public CCLayer{
public:
    CREATE_FUNC(ProceedController);
    static ProceedController* Singleton();
    bool init();
};

#endif /* defined(__BlackWhiteRush__ProceedController__) */
