//
//  ProceedController.cpp
//  BlackWhiteRush
//
//  Created by 何遵祖 on 13-12-29.
//
//

#include "ProceedController.h"
#include "ProceedView.h"
#include "SimpleAudioEngine.h"
#include "SoundResources.h"

using namespace CocosDenshion;

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
    jump->setScale(1.6f);
    jump->setPosition(ccp(size.width - jump->getContentSize().width - 50, 100));
    
    //fever按钮
    CCMenuItemSprite *fever = this->instanceButton("Fever_1_1.png", "Fever_1_1.png",menu_selector(ProceedController::gameFeverFunc));
    fever->setScale(1.5f);
    fever->setOpacity(0.0f);
    fever->setPosition(ccp(jump->getPosition().x - fever->getContentSize().width * 2.0,
                           jump->getPosition().y));
    
    CCMenu *menu = CCMenu::create(pause,jump,fever,NULL);
    menu->setPosition(0, 0);
    this->addChild(menu,80);
    
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
        SimpleAudioEngine::sharedEngine()->playEffect(S_SELECT);
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
    if (GameLogic::Singleton()->getPView()->getRole()->getState() != Role_Death &&
        GameLogic::Singleton()->getPView()->getRole()->getSjState() == Jump_None) {
        GameLogic::Singleton()->getPView()->getRole()->setSjState(Jump_Once);
    }else if(GameLogic::Singleton()->getPView()->getRole()->getSjState() == Jumping_Once){
        GameLogic::Singleton()->getPView()->getRole()->setSjState(Jump_Second);
    }
}

void ProceedController::gameFeverFunc(){
    CCLOG("+================game fever func====================+");
    if (GameLogic::Singleton()->getFeverState() != Fever_enable) return;
    
    //加倍
    //增加倍数
    if(GameLogic::Singleton()->getMultiple() <= 80){
        SimpleAudioEngine::sharedEngine()->playEffect(S_FEVER_1);
        GameLogic::Singleton()->setMultiple(GameLogic::Singleton()->getMultiple() + 10);
    }
    GameLogic::Singleton()->getPView()->getProgress()->setPercentage(0.0f);
    //取消fever状态
    GameLogic::Singleton()->setFeverState(Fever_doing);
}