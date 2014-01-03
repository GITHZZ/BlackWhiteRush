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
}

bool ProceedScene::init(){
    if (!CCScene::init()) {
        return false;
    }
    
    ProceedView *proView = ProceedView::Singleton();
    this->addChild(proView,ViewZOrder);
    
    ProceedController *pController = ProceedController::Singleton();
    this->addChild(pController,ControllerZOrder);
    
    //游戏逻辑关联主渲染场景
    GameLogic::Singleton()->setPView(proView);
    
    return true;
}