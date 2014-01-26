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
    _isTrigger = true;
    _boxRect = CCSizeMake(0.5,0.5);
    _standardPoint = ccp(150, 300);
    
    return true;
}

void Role::onCollisionEnter(GameObject* collision){
    CCLOG("++===============Collision Enter======================++");
    //如果撞到障碍物 就把Fever状态设计成enable
    if (collision->getObjType() == Object_Block && _rState != Role_Bruise) {//障碍物
        Barrier* barrier = dynamic_cast<Barrier*>(collision);
        switch (barrier->getBarrierType()) {
            case Barrier_Gear:
            case Barrier_Stab:
            case Barrier_Stone:
            case Barrier_Rocket:{
                _rState = Role_Bruise;
                float bloodValue = GameLogic::Singleton()->getBlood();
                GameLogic::Singleton()->setBlood(bloodValue -= bloodValue * 0.3);//30%
                CCCallFunc* callFunc = CCCallFunc::create(this, callfunc_selector(Role::bruiseFunc));
                this->runAction(CCSequence::create(CCRepeat::create(CCFadeIn::create(0.3f),3.0f),callFunc));
            }
                break;
            case Barrier_Step:
                break;
            default:
                CCLOG("+==============fail to get Barrier Type=================+");
                exit(EXIT_FAILURE);
                break;
        }
    }else if(collision->getObjType() == Object_Prop){//道具，添加到道具栏中
        Props* prop = dynamic_cast<Props*>(collision);
        switch (prop->getPropType()) {
            case Prop_Sprint:
            case Prop_Blood:
            case Prop_Wave:{
                //粒子特效
//                CCParticleSystem* particle = CCParticleSystemQuad::create("StarParticle.plist");
//                particle->setPosition(prop->getBodyPosition());
//                GameLogic::Singleton()->getPView()->addChild(particle);
                //先在工具栏设置类型
                GameLogic::Singleton()->getPView()->drawPropToBox(prop->getPropType());
                //删除该道具并且放入到工具栏中
                GameLogic::Singleton()->getPView()->removeChild(prop);
                GameLogic::Singleton()->getPView()->destroyBody(prop);
                GameLogic::Singleton()->deleteObject(prop);
            }
                break;
            default:
                CCLOG("+==============fail to get Prop Type=================+");
                exit(EXIT_FAILURE);
                break;
        }
    }
}

void Role::bruiseFunc(){
    _rState = Role_Move;
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

void Role::skillAction(SkillState sk){
    switch (sk) {
        case Skill_Blood:{
            CCParticleSystem* particle = CCParticleSystemQuad::create("Teleport.plist");
            particle->setPosition(this->getBodyPosition());
            GameLogic::Singleton()->getPView()->addChild(particle);
            
            float bloodValue = GameLogic::Singleton()->getBlood();
            GameLogic::Singleton()->setBlood(bloodValue += bloodValue * 0.1);//10%
            CCLOG("+=================Blood Skill Open===============+");
        }
            break;
        case Skill_Sprint:{
//            CCParticleSystem* particle = CCParticleSystemQuad::create("Sprint.plist");
//            particle->setPosition(ccpAdd(this->getBodyPosition(),ccp(10,0)));
//            GameLogic::Singleton()->getPView()->addChild(particle);
            _pB2Body->ApplyLinearImpulse(b2Vec2(40,0),_pB2Body->GetWorldCenter());
            _pB2Body->SetAwake(false);
           
            CCLOG("+=================Sprint Skill Open===============+");
        }
            break;
        case Skill_Wave:
            
            CCLOG("+=================Wave Skill Open===============+");
            break;
        default:
            break;
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