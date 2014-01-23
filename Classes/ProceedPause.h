//
//  ProceedPause.h
//  BlackWhiteRush
//
//  Created by 何遵祖 on 14-1-6.
//
//

#ifndef __BlackWhiteRush__ProceedPause__
#define __BlackWhiteRush__ProceedPause__

#include "cocos2d.h"
#include "GameLogic.h"

USING_NS_CC;

class ProceedPause : public CCSprite{
public:
    ~ProceedPause();
    virtual bool init();
    CREATE_FUNC(ProceedPause);

    CCMenuItemSprite* instanceButton(const char *unselected, const char *selected,SEL_MenuHandler selector);
    
    void backBtnFunc();
    void homeBtnFunc();
    void musicBtnFunc();
    void restartBtnFunc();
private:
    static ProceedPause* _singletonPause;
    
    CC_SYNTHESIZE(bool,_fristAdd,FristAdd);
};

#endif /* defined(__BlackWhiteRush__ProceedPause__) */
