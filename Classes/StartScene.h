//
//  StartScene.h
//  BlackWhiteRush
//
//  Created by 何遵祖 on 14-1-23.
//
//

#ifndef __BlackWhiteRush__StartScene__
#define __BlackWhiteRush__StartScene__

#include "cocos2d.h"
USING_NS_CC;
#define BACKGROUND_TAG 100

class StartAnim : public CCLayer{
public:
    static CCScene* scene();
    CREATE_FUNC(StartAnim);
    bool init();
    void sceneStartAnimEnd();
    void backgroundWaveEnd();
};

class StartScene : public CCLayer{
public:
    static CCScene* scene();
    CREATE_FUNC(StartScene);
    bool init();
    void preLoadSoundResources();
    void playBackgroundMusic();
    void gameStart();
    void addSelectButton();
    
    virtual void onEnter();
    virtual void onExit();
    virtual bool ccTouchBegan(CCTouch* pTouch,CCEvent* pEvent);
};

#endif /* defined(__BlackWhiteRush__StartScene__) */
