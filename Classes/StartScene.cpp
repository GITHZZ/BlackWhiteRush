//
//  StartScene.cpp
//  BlackWhiteRush
//
//  Created by 何遵祖 on 14-1-23.
//
//

#include "StartScene.h"

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
    
    
    
    return true;
}