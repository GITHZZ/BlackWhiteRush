//
//  Role.cpp
//  BlackWhiteRush
//
//  Created by 何遵祖 on 13-12-30.
//
//

#include "Role.h"
#include "ProceedView.h"

Role* Role::instance(const char* file){
    Role *role = new Role();
    if (role && role->inits(file)) {
        role->autorelease();
        return role;
    }
    CC_SAFE_DELETE(role);
    return NULL;
}

bool Role::inits(const char* file){
    CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(file);
    if (!this->initWithSpriteFrame(frame)) {
        return false;
    }
    
    _objType = Object_Role;
    _speed = 100;
    _rState = Role_Move;
    _skState = Skill_None;
    _boxRect = CCSizeMake(0.5,0.5);
    _standardPoint = ccp(150, 300);
    
    return true;
}

void Role::onCollisionEnter(GameObject* collision){
    CCLOG("++===============Collision Enter======================++");
}

void Role::update(float dt){
    
    //根据状态进行跳跃动作
    this->jumpAction();
    
    //修正位置如果不是在标准位置的话,调整,横坐标
    if (_rState == Role_Move) {
        //获取实际坐标
        float realPointX = this->getBodyPosition().x;
        float subPointX = _standardPoint.x - realPointX;
        if (subPointX != 0) {//有偏差
            this->setBodyPosition(ccpAdd(this->getBodyPosition(),ccp(subPointX * dt,0)));
        }
    }
}

void Role::jumpAction(){
    if (_rState == Role_JumpOnce) {
        _pB2Body->ApplyLinearImpulse(b2Vec2(0,8),_pB2Body->GetPosition());
        _rState = Role_JumpingOnce;
    }
    if (_rState == Role_JumpSecond) {
        _pB2Body->ApplyLinearImpulse(b2Vec2(0,7),_pB2Body->GetPosition());
        _rState = Role_JumpingSecond;
    }
    if (_rState == Role_JumpingSecond &&
        _pB2Body->GetPosition().y * _ptmRadio <= this->getContentSize().height + SUB_HEIGHT &&
        _pB2Body->GetLinearVelocity().y <= 0) {
        //接触地面就变回Role_Move状态
        _rState = Role_Move;
        //旋转角度复原
        this->setBodyRotation(0);
    }
    if (_rState == Role_JumpingOnce &&
        _pB2Body->GetPosition().y * _ptmRadio <= this->getContentSize().height + SUB_HEIGHT &&
        _pB2Body->GetLinearVelocity().y <= 0) {
        //接触地面就变回Role_Move状态
        _rState = Role_Move;
        //旋转角度复原
        this->setBodyRotation(0);
    }
    if (_rState != Role_Move && _rState != Role_Death) {
        float newAngle = CC_RADIANS_TO_DEGREES((CC_DEGREES_TO_RADIANS(_pB2Body->GetAngle()) - 0.001f));
        _pB2Body->SetTransform(_pB2Body->GetPosition(), newAngle);
    }
}