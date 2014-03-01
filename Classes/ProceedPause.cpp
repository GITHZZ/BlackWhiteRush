//
//  ProceedPause.cpp
//  BlackWhiteRush
//
//  Created by 何遵祖 on 14-1-6.
//
//

#include "ProceedPause.h"
#include "ProceedScene.h"
#include "SimpleAudioEngine.h"
#include "SoundResources.h"
#include "StartScene.h"

using namespace CocosDenshion;

ProceedPause::~ProceedPause(){
}

bool ProceedPause::init(){
    if (!CCSprite::init()) {
        return false;
    }
    
    _fristAdd = true;
    
    //添加背景
    CCSprite* bg = CCSprite::createWithSpriteFrameName("pauseBg.png");
    this->addChild(bg);
    
    //添加按钮
    //继续
    CCMenuItemSprite* backBtn = this->instanceButton("back1.png","back2.png",menu_selector(ProceedPause::menuButtonFunc));
    backBtn->setTag(PA_BACKBTN_TAG);
    
    //主页
    CCMenuItemSprite* homeBtn = this->instanceButton("home.png", "home2.png",menu_selector(ProceedPause::menuButtonFunc));
    homeBtn->setTag(PA_HOMEBTN_TAG);
    
    //重新开始
    CCMenuItemSprite* restartBtn = this->instanceButton("restart.png", "restart.png", menu_selector(ProceedPause::menuButtonFunc));
    restartBtn->setScale(1.3f);
    restartBtn->setTag(PA_RESTARTBTN_TAG);
    
    CCMenu* menu = CCMenu::create(backBtn,homeBtn,restartBtn,NULL);
    menu->setPosition(CCPointZero);
    menu->alignItemsHorizontallyWithPadding(5);
    this->addChild(menu);
    
    return true;
}

CCMenuItemSprite* ProceedPause::instanceButton(const char *unselected, const char *selected,SEL_MenuHandler selector){
    CCSpriteFrame* frame1 = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(unselected);
    CCSprite* unselectSpr = CCSprite::createWithSpriteFrame(frame1);
    
    CCSpriteFrame* frame2 = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(selected);
    CCSprite* selectSpr = CCSprite::createWithSpriteFrame(frame2);
    
    CCMenuItemSprite* selBtn = CCMenuItemSprite::create(unselectSpr, selectSpr, this, selector);
    return selBtn;
}

void ProceedPause::menuButtonFunc(CCObject* obj){
    CCMenuItemSprite* menuObj = dynamic_cast<CCMenuItemSprite*>(obj);
    SimpleAudioEngine::sharedEngine()->playEffect(S_OK);
    
    switch (menuObj->getTag()) {
        case PA_BACKBTN_TAG:{
            if (GameLogic::Singleton()->getState() == State_Pause) {
                GameLogic::Singleton()->setState(State_Playing);
            }
        }
            break;
        case PA_HOMEBTN_TAG:{
            GameLogic::Singleton()->end();
            
            CCDirector::sharedDirector()->purgeCachedData();
            CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(2.5f, StartScene::scene()));

        }
            break;
        case PA_RESTARTBTN_TAG:{
            SimpleAudioEngine::sharedEngine()->playEffect("ok.mp3");
            
            GameLogic::Singleton()->end();
            
            CCDirector::sharedDirector()->purgeCachedData();
            CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(2.5f, ProceedScene::create()));

        }
            break;
        default:
            break;
    }
}