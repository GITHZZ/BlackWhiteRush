//
//  ProceedEnd.h
//  BlackWhiteRush
//
//  Created by 何遵祖 on 14-2-21.
//
//

#ifndef __BlackWhiteRush__ProceedEnd__
#define __BlackWhiteRush__ProceedEnd__

#include "cocos2d.h"

USING_NS_CC;

#define PE_TAG_RESTART 600
#define PE_TAG_HOME    601

class ProceedEnd : public CCSprite{
public:
    ~ProceedEnd();
    virtual bool init();
    CREATE_FUNC(ProceedEnd);
    
    void update();
    void addButtons();
    
    CCMenuItemSprite* instanceButton(const char* unselected,const char* selected,SEL_MenuHandler selector);
    void menuButtomFunc(CCObject* obj);
private:
    static ProceedEnd* _singletonEnd;
    CCLabelBMFont* scoreStr;
    int animScore;//播放动画用的分数
};

#endif /* defined(__BlackWhiteRush__ProceedEnd__) */
