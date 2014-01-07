//
//  ProceedController.cpp
//  BlackWhiteRush
//
//  Created by 何遵祖 on 13-12-29.
//
//

#include "ProceedController.h"

ProceedController* ProceedController::_singletonController = NULL;

ProceedController* ProceedController::Singleton(){
    CCLOG("+=============PROCEEDCONTROLLER SINGLETON==================+");
    if (!_singletonController) {
        _singletonController = ProceedController::create();
    }
    return _singletonController;
}

bool ProceedController::init(){
    if (!CCLayer::init()) {
        return false;
    }
    
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    
    //暂停按钮
    CCMenuItemSprite *pause = this->instanceButton("pause1.png", "pause2.png", menu_selector(ProceedController::gamePauseFunc));
    pause->setScale(1.5f);
    pause->setPosition(ccp(size.width - pause->getContentSize().width,
                           size.height - pause->getContentSize().height));
    
    //跳跃按钮
    CCMenuItemSprite *jump = this->instanceButton("control_btn1.png", "control_btn2.png", menu_selector(ProceedController::gameJumpFunc));
    
    
    CCMenu *menu = CCMenu::create(pause,NULL);
    menu->setPosition(0, 0);
    this->addChild(menu,100);
    
    return true;
}

CCMenuItemSprite* ProceedController::instanceButton(const char *unselected, const char *selected,SEL_MenuHandler selector){
    CCSpriteFrame *frame1 = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(unselected);
    CCSprite* unselectSpr = CCSprite::createWithSpriteFrame(frame1);

    CCSpriteFrame *frame2 = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(selected);
    CCSprite* selectSpr = CCSprite::createWithSpriteFrame(frame2);
    
    CCMenuItemSprite* selBtn = CCMenuItemSprite::create(unselectSpr, selectSpr, this, selector);
    return selBtn;
}

void ProceedController::gamePauseFunc(){
    CCLOG("+=============pause bottom press down==============+");
    if (GameLogic::Singleton()->getState() == State_Playing) {
        GameLogic::Singleton()->setState(State_Pause);
    }
}

void ProceedController::gameJumpFunc(){
    
}