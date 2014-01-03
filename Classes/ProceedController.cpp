//
//  ProceedController.cpp
//  BlackWhiteRush
//
//  Created by 何遵祖 on 13-12-29.
//
//

#include "ProceedController.h"

static ProceedController* _singletonController = NULL;

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
    
    return true;
}