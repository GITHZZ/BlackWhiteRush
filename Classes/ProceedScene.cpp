//
//  ProceedScene.cpp
//  BlackWhiteRush
//
//  Created by 何遵祖 on 13-12-29.
//
//

#include "ProceedScene.h"
#include "ProceedView.h"
#include "ProceedController.h"

ProceedScene::~ProceedScene(){
    CCLOG("+==========PROCEEDSCENE RELEASE=============+");
    GameLogic::Singleton()->end();
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFrameByName("gElement.plist");
}

bool ProceedScene::init(){
    if (!CCScene::init()) {
        return false;
    }
    
    //将精灵表中的.plist文件加进缓冲区
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("gElement.plist");
    
    ProceedView *proView = ProceedView::Singleton();
    this->addChild(proView,ViewZOrder);
    
    ProceedController *pController = ProceedController::Singleton();
    this->addChild(pController,ControllerZOrder);
    
    //游戏逻辑关联主渲染场景
    GameLogic::Singleton()->setPView(proView);
    
    return true;
}