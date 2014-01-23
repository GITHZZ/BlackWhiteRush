//
//  ProceedPause.cpp
//  BlackWhiteRush
//
//  Created by 何遵祖 on 14-1-6.
//
//

#include "ProceedPause.h"
#include "ProceedScene.h"

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
    CCMenuItemSprite* backBtn = this->instanceButton("back1.png","back2.png",menu_selector(ProceedPause::backBtnFunc));
    //主页
    CCMenuItemSprite* homeBtn = this->instanceButton("home.png", "home2.png",menu_selector(ProceedPause::homeBtnFunc));
    //重新开始
    CCMenuItemSprite* restartBtn = this->instanceButton("restart.png", "restart.png", menu_selector(ProceedPause::restartBtnFunc));
    restartBtn->setScale(1.3f);
    
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

void ProceedPause::backBtnFunc(){
    if (GameLogic::Singleton()->getState() == State_Pause) {
        GameLogic::Singleton()->setState(State_Playing);
    }
}

void ProceedPause::homeBtnFunc(){
    //CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFrameByName("gElement.plist");
}

void ProceedPause::musicBtnFunc(){
    
}

void ProceedPause::restartBtnFunc(){
    
    GameLogic::Singleton()->end();
    
    CCDirector::sharedDirector()->purgeCachedData();
    CCDirector::sharedDirector()->replaceScene(ProceedScene::create());
}