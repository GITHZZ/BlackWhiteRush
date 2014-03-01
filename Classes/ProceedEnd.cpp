//
//  ProceedEnd.cpp
//  BlackWhiteRush
//
//  Created by 何遵祖 on 14-2-21.
//
//

#include "ProceedEnd.h"
#include "ProceedScene.h"
#include "StartScene.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

ProceedEnd::~ProceedEnd(){
}

bool ProceedEnd::init(){
    if (!CCSprite::init()) {
        return false;
    }
    
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    CCSprite* bg = CCSprite::createWithSpriteFrameName("GameEnd.png");
    bg->setScale(2.0f);
    bg->setPosition(ccp(size.width/2, size.height/2));
    //bg->runAction(CCScaleTo::create(0.3f, 2.0f));
    this->addChild(bg);
    
    
    scoreStr = CCLabelBMFont::create("000000", "ballpark-32-hd.fnt");
    scoreStr->setPosition(ccp(570,size.height/2+20));
    this->addChild(scoreStr);
    
    animScore = 0;
    
    this->schedule(schedule_selector(ProceedEnd::update));
    
    return true;
}

void ProceedEnd::update(){
    if (animScore <= 100) {
        animScore ++;
        scoreStr->setString(CCString::createWithFormat("%d",arc4random() % 10000 + 10000)->getCString());
    }else{
        //设置最后的分数
        scoreStr->setString(CCString::createWithFormat("%d",10000)->getCString());
        
        this->addButtons();
        
        this->unschedule(schedule_selector(ProceedEnd::update));
    }
    
}

void ProceedEnd::addButtons(){
    CCMenuItemSprite* repeat = this->instanceButton("restart.png", "restart.png",menu_selector(ProceedEnd::menuButtomFunc));
    repeat->runAction(CCSequence::create(CCScaleTo::create(0.2f, 1.3f),
                                         CCScaleTo::create(0.2f, 1.0f),
                                         NULL));
    repeat->setTag(PE_TAG_RESTART);
    
    CCMenuItemSprite* home = this->instanceButton("home.png", "home.png",menu_selector(ProceedEnd::menuButtomFunc));
    home->setScale(0.8f);
    home->runAction(CCSequence::create(CCScaleTo::create(0.2f, 1.1f),
                                       CCScaleTo::create(0.2f, 0.8f),
                                       NULL));
    home->setTag(PE_TAG_HOME);
    
    CCMenu* menu = CCMenu::create(repeat,home,NULL);
    menu->setPosition(ccp(370, 150));
    menu->alignItemsHorizontallyWithPadding(10);
    this->addChild(menu);
}

CCMenuItemSprite* ProceedEnd::instanceButton(const char *unselected, const char *selected, SEL_MenuHandler selector){
    CCSpriteFrame* frame1 = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(unselected);
    CCSprite* unselectSpr = CCSprite::createWithSpriteFrame(frame1);
    
    CCSpriteFrame* frame2 = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(selected);
    CCSprite* selectSpr = CCSprite::createWithSpriteFrame(frame2);
    
    CCMenuItemSprite* selBtn = CCMenuItemSprite::create(unselectSpr, selectSpr, this, selector);
    return selBtn;
}

void ProceedEnd::menuButtomFunc(cocos2d::CCObject *obj){
    CCMenuItemSprite* tmpBtn = dynamic_cast<CCMenuItemSprite*>(obj);
    SimpleAudioEngine::sharedEngine()->playEffect("ok.mp3");
    
    switch (tmpBtn->getTag()) {
        case PE_TAG_RESTART:{
          
            
            GameLogic::Singleton()->end();
            
            CCDirector::sharedDirector()->purgeCachedData();
            CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(2.5f, ProceedScene::create()));
        }
            break;
        case PE_TAG_HOME:{
            SimpleAudioEngine::sharedEngine()->playEffect("ok.mp3");
            
            GameLogic::Singleton()->end();
            
            CCDirector::sharedDirector()->purgeCachedData();
            CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(2.5f, StartScene::scene()));
        }
            break;
        default:
            break;
    }
}

