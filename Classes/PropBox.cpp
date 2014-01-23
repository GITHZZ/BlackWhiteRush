//
//  PropBox.cpp
//  BlackWhiteRush
//
//  Created by 何遵祖 on 14-1-20.
//
//

#include "PropBox.h"
#include "GameLogic.h"
#include "ProceedView.h"

PropBox* PropBox::instance(const char *file,int index){
    PropBox* propBox = new PropBox();
    if (propBox && propBox->inits(file,index)) {
        propBox->autorelease();
        return propBox;
    }
    CC_SAFE_DELETE(propBox);
    return NULL;
}

bool PropBox::inits(const char* file,int index){
    CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(file);
    if (!this->initWithSpriteFrame(frame)) {
        return false;
    }
    
    _index = index;
    _propType = Prop_None;
    
    this->setScale(0.7f);
    
    return true;
}

PropType PropBox::getType() const{
    return _propType;
}

void PropBox::drawElementFromType(PropType ty){
    if (_propType != Prop_None && ty != Prop_None) {//如果目前的道具类型是空而且设置类型不是空
        return;
    }
    _propType = ty;
    
    this->renderObjFromType(ty);
}

void PropBox::renderObjFromType(PropType ty){
    const char* fileName = NULL;
    //根据具体类型进行渲染
    switch (ty) {
        case Prop_Blood:
            if (fileName == NULL) fileName = "B.png";
        case Prop_Sprint:
            if (fileName == NULL) fileName = "s.png";
        case Prop_Wave:{
            if (fileName == NULL) fileName = "w.png";
            CCSprite* propPic = CCSprite::createWithSpriteFrameName(fileName);
            propPic->setAnchorPoint(ccp(0,0));
            propPic->setScale(2.0f);
            this->addChild(propPic,-1);
        }
            break;
        default:
            break;
    }
}

bool PropBox::pointInRect(CCPoint pos){
    CCRect boxRect = CCRectMake(this->getPosition().x - this->getContentSize().width/2,
                                this->getPosition().y - this->getContentSize().height/2,
                                this->getContentSize().width,
                                this->getContentSize().height);
    return boxRect.containsPoint(pos);
}

void PropBox::onEnter(){
    CCSprite::onEnter();
    CCDirector::sharedDirector()->getTouchDispatcher()->addStandardDelegate(this, 0);
}

void PropBox::onExit(){
    CCSprite::onExit();
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}

void PropBox::ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent){
    CCTouch* touch = (CCTouch*)pTouches->anyObject();
    CCPoint location = touch->getLocationInView();
    location = CCDirector::sharedDirector()->convertToGL(location);
    
    if (this->pointInRect(location)) {
        //移除精灵并且设置为空
        //并且启动相应技能
        if (_propType != Prop_None) {
            this->removeAllChildren();
            switch (_propType) {
                case Prop_Blood:
                    GameLogic::Singleton()->getPView()->getRole()->skillAction(Skill_Blood);
                    break;
                case Prop_Sprint:
                    GameLogic::Singleton()->getPView()->getRole()->skillAction(Skill_Sprint);
                    break;
                case Prop_Wave:
                    GameLogic::Singleton()->getPView()->getRole()->skillAction(Skill_Wave);
                    break;
                default:
                    break;
            }
            _propType = Prop_None;
        }
        CCLOG("+============Touches Index %d================+",_index);
    }
}

void PropBox::ccTouchesMoved(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent){
    
}

void PropBox::ccTouchesEnded(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent){
    
}