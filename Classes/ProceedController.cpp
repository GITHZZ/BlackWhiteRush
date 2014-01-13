//
//  ProceedController.cpp
//  BlackWhiteRush
//
//  Created by 何遵祖 on 13-12-29.
//
//

#include "ProceedController.h"
#include "ProceedView.h"

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
    jump->setScale(1.5f);
    jump->setPosition(ccp(size.width - jump->getContentSize().width - 50, 100));
    
    //fever按钮
    CCMenuItemSprite *fever = this->instanceButton("fever.png", "fever.png",menu_selector(ProceedController::gameFeverFunc));
    fever->setScale(1.5f);
    fever->setPosition(ccp(jump->getPosition().x - fever->getContentSize().width * 1.5,
                           jump->getPosition().y));
    
    CCMenu *menu = CCMenu::create(pause,jump,fever,NULL);
    menu->setPosition(0, 0);
    this->addChild(menu,100);
    
//    CCSpriteFrame*feverFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("feverBg.png");
//    CCSprite *feverBg = CCSprite::createWithSpriteFrame(feverFrame);
//    
//    feverProgress = CCProgressTimer::create(feverBg);
//    feverProgress->setPercentage(10.0f);
//    feverProgress->setType(kCCProgressTimerTypeRadial);
//    feverProgress->setPosition(fever->getPosition());
//    
//    this->addChild(feverProgress);
    
    return true;
}

CCMenuItemSprite* ProceedController::instanceButton(const char *unselected, const char *selected,SEL_MenuHandler selector){
    CCSpriteFrame* frame1 = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(unselected);
    CCSprite* unselectSpr = CCSprite::createWithSpriteFrame(frame1);

    CCSpriteFrame* frame2 = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(selected);
    CCSprite* selectSpr = CCSprite::createWithSpriteFrame(frame2);
    
    CCMenuItemSprite* selBtn = CCMenuItemSprite::create(unselectSpr, selectSpr, this, selector);
    return selBtn;
}

void ProceedController::gamePauseFunc(){
    CCLOG("+=============pause bottom press down==============+");
    if (GameLogic::Singleton()->getState() == State_Playing) {
        GameLogic::Singleton()->setState(State_Pause);
    }else if(GameLogic::Singleton()->getState() == State_Pause){
        GameLogic::Singleton()->setState(State_Playing);
    }
    
    //若是多人进行状态
    if (GameLogic::Singleton()->getModel() == Model_Multiple) {
        //广播通知
    }
}

void ProceedController::gameJumpFunc(){
    CCLOG("+==============game jump press down=================+");
    if (GameLogic::Singleton()->getPView()->getRole()->getState() == Role_Move) {
        GameLogic::Singleton()->getPView()->getRole()->setState(Role_JumpOnce);
    }else if(GameLogic::Singleton()->getPView()->getRole()->getState() == Role_JumpingOnce){
        GameLogic::Singleton()->getPView()->getRole()->setState(Role_JumpSecond);
    }
}

void ProceedController::gameFeverFunc(){
    CCLOG("+================game fever func====================+");
}