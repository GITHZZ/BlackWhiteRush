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

#define PA_BACKBTN_TAG     500
#define PA_HOMEBTN_TAG     501
#define PA_RESTARTBTN_TAG  502

USING_NS_CC;

class ProceedPause : public CCSprite{
public:
    ~ProceedPause();
    virtual bool init();
    CREATE_FUNC(ProceedPause);

    CCMenuItemSprite* instanceButton(const char *unselected, const char *selected,SEL_MenuHandler selector);
    void menuButtonFunc(CCObject* obj);

private:
    //static ProceedPause* _singletonPause;
    
    CC_SYNTHESIZE(bool,_fristAdd,FristAdd);
};

#endif /* defined(__BlackWhiteRush__ProceedPause__) */
