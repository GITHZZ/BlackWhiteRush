//
//  GameLogic.cpp
//  BlackWhiteRush
//
//  Created by 何遵祖 on 13-12-27.
//
//

#include "GameLogic.h"
#include "ProceedView.h"

static GameLogic *_singletonObject = NULL;

GameLogic* GameLogic::Singleton(){
    if (!_singletonObject) {
        _singletonObject = new GameLogic();
        _singletonObject->init();
    }
    return _singletonObject;
}

GameLogic::~GameLogic(){
    CCLOG("+===========GAMELOGIC RELEASE=============+");
    
    CC_SAFE_RELEASE(_pScheduler);
    CC_SAFE_RELEASE(_objects);
    
    _singletonObject = NULL;
}

void GameLogic::end(){
    if (_pScheduler) {
        _pScheduler->unscheduleSelector(schedule_selector(GameLogic::update), this);
    }
    CC_SAFE_DELETE(_singletonObject);
}

void GameLogic::init(){
    _state = State_Playing;
    _currentModel = Model_Single;
    
    _proceedview = NULL;

    _objects = new CCArray();
    CC_SAFE_RETAIN(_objects);
    _objects->init();
    
    /*初始化分数倍数*/
    _score = 0;
    _multiple = 30;
    
    /*获取计时器*/
    _pScheduler = CCDirector::sharedDirector()->getScheduler();
    CC_SAFE_RETAIN(_pScheduler);
    _pScheduler->scheduleSelector(schedule_selector(GameLogic::update), this, UPDATE_INTERVAL,false);
}

void GameLogic::update(float dt){
    //输出游戏状态
    if (LOG_GAMESTATE_ENABLED) this->logGameState(_state);
    //根据游戏状态实现不同状态
    switch (_state) {
        case State_Start: {
            //游戏时间倒数(PS:暂停结束后也是需要先从这个状态开始)
            //时间倒数结束后直接转入State_Playing状态
        }
            break;
        case State_Playing:{
            //通知ProcceedView进行box2D更新
            _proceedview->update(dt);
            //游戏进行状态,主要是场景循环,玩家可以运动和道具加障碍物的产生,Fever逻辑,分数
            //更新分数
            this->updateScore();
            //碰撞检测监听事件
            this->collisionListener();
            //更新背景
            //产生障碍物,道具
            //移动背景,障碍物和道具
            this->moveGameObject(dt);
            
            if (_currentModel == Model_Multiple) {
                //网络连接
            }
        }
            break;
        case State_Pause:{
            //游戏暂停状态,单机可以进行暂停,对战可以选择此功能
        }
            break;
        case State_End:{
            //游戏结束,弹出结果,分数.
            //如果是多人游戏,就是列出分数并且选出最高分
        }
            break;
        default:
            break;
    }
}

void GameLogic::moveGameObject(float dt){
    for (int i = 0; i < _objects->count(); i++) {
        GameObject *obj = dynamic_cast<GameObject*>(_objects->objectAtIndex(i));
        //如果是主角不用移动
        if (obj->getObjType() == Object_Role) {
            continue;
        }
        //如果是背景对象而且位置已经超过了就删除并且添加新的
        if (obj->getObjType() == Object_Background &&
            obj->getBodyPosition().x <= -obj->getContentSize().width) {
            CCLOG("++++++++++remove background+++++++++++++++");
            
            GameObject *lastObj = this->popObjectFromType(Object_Background);
            _proceedview->drawBackground(ccp(lastObj->getBodyPosition().x + lastObj->getContentSize().width * obj->getScaleX(),
                                             lastObj->getBodyPosition().y));

            _proceedview->removeChild(obj);
            this->deleteObject(obj);
        }
        
        obj->setBodyPosition(ccp(obj->getBodyPosition().x - BACKGROUND_SPEED * dt, obj->getBodyPosition().y));
    }
}

void GameLogic::updateScore(){
    _score += _score * _multiple;
}

void GameLogic::pushObject(GameObject *gameObject){
    _objects->addObject(gameObject);
}

void GameLogic::deleteObject(GameObject* gameObject){
    _objects->removeObject(gameObject);
}

GameObject* GameLogic::popObjectFromType(ObjectType t){
    for (int i = _objects->count() - 1 ; i >= 0; i --) {
        GameObject* object = dynamic_cast<GameObject*>(_objects->objectAtIndex(i));
        if (object->getObjType() == t) {
            return object;
        }
    }
    CCLOG("+==================Fail to get Object from type======================+");
    return NULL;
}

void GameLogic::collisionListener(){
    if (_objects && _objects->count() > 0) {
        for (int i = 0; i < _objects->count(); i++) {
            GameObject* arr_obj = dynamic_cast<GameObject*>(_objects->objectAtIndex(i));
            
            if (arr_obj->getObjType() == Object_Role) {//主角
                for (int j = 0; j < _objects->count(); j++) {
                    
                    GameObject* unRoleObj = dynamic_cast<GameObject*>(_objects->objectAtIndex(0));
                    
                    if (unRoleObj->getObjType() != Object_Role) {//道具或者障碍物
                        arr_obj->checkCollision(unRoleObj);
                    }
                }
            }
        }
    }
}

void GameLogic::logGameState(GameState s){
    switch (s) {
        case 0:
            CCLOG("State_Playing");
            break;
        case 1:
            CCLOG("State_Pause");
            break;
        case 2:
            CCLOG("State_Start");
            break;
        case 3:
            CCLOG("State_End");
            break;
        default:
            break;
    }
}