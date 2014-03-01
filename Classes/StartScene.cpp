//
//  StartScene.cpp
//  BlackWhiteRush
//
//  Created by 何遵祖 on 14-1-23.
//
//

#include "StartScene.h"
#include "SimpleAudioEngine.h"
#include "SoundResources.h"
#include "ProceedScene.h"

using namespace CocosDenshion;

CCScene* StartAnim::scene(){
    CCScene* scene = CCScene::create();
    StartAnim* layer = StartAnim::create();
    scene->addChild(layer);
    return scene;
}

bool StartAnim::init(){
    if (!CCLayer::init()) {
        return false;
    }
    
    //将精灵表中的.plist文件加进缓冲区
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("gElement.plist");

    //CCSize size = CCDirector::sharedDirector()->getWinSize();
    CCSize size = CCDirector::sharedDirector()->getVisibleSize();
    
    CCSprite* bg = CCSprite::createWithSpriteFrameName("bg_w.png");
    bg->setPosition(ccp(size.width/2,size.height/2));
    bg->setOpacity(0);
    bg->setScaleX(2.5f);
    bg->setScaleY(2.0f);
    
    bg->runAction(CCSpawn::create(CCFadeIn::create(4.0f),
                                  CCWaves3D::create(4.0f, CCSizeMake(10, 10), 2, 20),
                                  NULL));
    
    this->addChild(bg,-1,BACKGROUND_TAG);
    
    CCLabelBMFont* bwGame = CCLabelBMFont::create("BlackWhiteGame", "ballpark-32-hd.fnt");
    bwGame->setPosition(ccp(size.width/4 - 40, size.height/4));
    bg->addChild(bwGame);
    
    //预加载
    SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic(S_BACKGROUND);
    
    this->schedule(schedule_selector(StartAnim::backgroundWaveEnd), 1.0f/60.0f);
    
    return true;
}

void StartAnim::sceneStartAnimEnd(){
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFrameByName("gElement.plist");
    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(2.5f, StartScene::scene()));
}

void StartAnim::backgroundWaveEnd(){
    CCNode* bg_node = this->getChildByTag(BACKGROUND_TAG);
    if (bg_node->numberOfRunningActions() == 0 &&
        bg_node->getGrid() != NULL) {
        bg_node->setGrid(NULL);
        
        this->sceneStartAnimEnd();
    }
}

CCScene* StartScene::scene(){
    CCScene* scene = CCScene::create();
    StartScene* layer = StartScene::create();
    scene->addChild(layer);
    return scene;
}

bool StartScene::init(){
    if (!CCLayer::init()) {
        return false;
    }
    
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("gElement.plist");
    
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    
    CCSprite* bg = CCSprite::createWithSpriteFrameName("bg_w.png");
    bg->setPosition(ccp(size.width/2,size.height/2));
    bg->setScaleX(2.5f);
    bg->setScaleY(2.0f);
    this->addChild(bg,-1,BACKGROUND_TAG);
    
    CCLabelBMFont* bwTitle = CCLabelBMFont::create("BlackWhiteRush", "ballpark-32-hd.fnt");
    bwTitle->setPosition(ccp(size.width/2, size.height/2 + 100));
    bwTitle->setScale(1.5f);
    this->addChild(bwTitle);
    
    CCSprite* play_label = CCSprite::createWithSpriteFrameName("play.png");
    play_label->setPosition(ccp(size.width/2, size.height/2 - 100));
    play_label->setScale(0.5f);
    this->addChild(play_label);
    
    CCSprite* play_shade = CCSprite::createWithSpriteFrameName("play.png");
    play_label->setScale(0.5f);
    play_shade->setPosition(play_label->getPosition());
    play_shade->runAction(CCRepeatForever::create(
                          CCSequence::create(
                                             CCSpawn::create(CCFadeOut::create(1.0f),
                                                             CCScaleTo::create(1.0f, 1.0f),
                                                             NULL),
                                             CCScaleTo::create(1.0f,0.5f),
                                             NULL)));
    
    this->runAction(CCSequence::create(CCDelayTime::create(1.5f),
                                       CCCallFunc::create(this, callfunc_selector(StartScene::playBackgroundMusic)),
                                       NULL));
    this->addChild(play_shade);
    
    this->preLoadSoundResources();
    
    return true;
}

void StartScene::onEnter(){
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
    CCLayer::onEnter();
}

void StartScene::onExit(){
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCLayer::onExit();
}

bool StartScene::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    CCLOG("+=====Touch Began=====+");
    this->gameStart();
    return true;
}

void StartScene::gameStart(){
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFrameByName("gElement.plist");
    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1.0f, ProceedScene::create()));
    SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
    SimpleAudioEngine::sharedEngine()->playEffect(S_OK);
}

void StartScene::addSelectButton(){
    
}

void StartScene::preLoadSoundResources(){
    SimpleAudioEngine::sharedEngine()->preloadEffect(S_JUMP);
    SimpleAudioEngine::sharedEngine()->preloadEffect(S_WAVE);
    SimpleAudioEngine::sharedEngine()->preloadEffect(S_FEVER_START);
    SimpleAudioEngine::sharedEngine()->preloadEffect(S_FEVER_END);
    SimpleAudioEngine::sharedEngine()->preloadEffect(S_FEVER_1);
    SimpleAudioEngine::sharedEngine()->preloadEffect(S_SELECT);
    SimpleAudioEngine::sharedEngine()->preloadEffect(S_OK);
    SimpleAudioEngine::sharedEngine()->preloadEffect(S_GAIN);
    SimpleAudioEngine::sharedEngine()->preloadEffect(S_SPRINT);
    SimpleAudioEngine::sharedEngine()->preloadEffect(S_RECIPROCAL);
}

void StartScene::playBackgroundMusic(){
    SimpleAudioEngine::sharedEngine()->playBackgroundMusic(S_BACKGROUND,true);
}