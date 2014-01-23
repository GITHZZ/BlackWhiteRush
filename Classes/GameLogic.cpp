//
//  GameLogic.cpp
//  BlackWhiteRush
//
//  Created by 何遵祖 on 13-12-27.
//
//

#include "GameLogic.h"
#include "ProceedView.h"

GameLogic* GameLogic::_singletonObject = NULL;

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
    
    CC_SAFE_RELEASE(_emitter);
    
    _objects->removeAllObjects();
    CC_SAFE_RELEASE_NULL(_objects);
    
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
    //_state = State_Start;
    _currentModel = Model_Single;
    
    _proceedview = NULL;

    _objects = new CCArray();
    CC_SAFE_RETAIN(_objects);
    _objects->init();
    
    /*初始化游戏中的物体发射器*/
    _emitter = new GameEmitter();
    CC_SAFE_RETAIN(_emitter);
    
    /*初始化分数倍数*/
    _score = 0;
    _multiple = 30;
    _feverState = Fever_unable;
    
    /*初始化间隔时间*/
    deltaTime = 1.0f;//1S
    curDeltaTime = 0.0f;
    
    //初始化血量
    _blood = 100;//100%
    
   
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
            //通知ProcceedView进行box2D更新
            _proceedview->update(dt);
            //移动背景,障碍物和道具
            this->moveGameObject(dt);
        }
            break;
        case State_Playing:{
            if (_currentModel == Model_Multiple) {
                //网络连接
            }
            //移动背景,障碍物和道具
            this->moveGameObject(dt);
            //游戏进行状态,主要是场景循环,玩家可以运动和道具加障碍物的产生,Fever逻辑,分数
            //通知ProcceedView进行box2D更新
            _proceedview->update(dt);
            //更新主角状态
            _proceedview->getRole()->update(dt);
            //绘制分数,fever非按钮部分,血条
            _proceedview->drawView();
            //如果是刚从暂停状态回复移除
            _proceedview->removePausePanel();
            _proceedview->removePauseShade();
            //更新Fever
            _proceedview->updateFever();
            if (_proceedview->getProgress()->getPercentage() >= 100) {
                //如果状态是不可按下就设成为可以按下
                if (_feverState == Fever_unable) _feverState = Fever_enable;
                if (_feverState == Fever_doing) {
                    _proceedview->getProgress()->setPercentage(0);
                    _multiple += 10;
                }
            }
            //更新分数
            this->updateScore();
            //更新血量
            this->updateBlood();
            //碰撞检测监听事件
            this->collisionListener();
            //更新背景
            //产生障碍物,道具 间隔时间 1S 产生
            curDeltaTime += dt;
            if (curDeltaTime >= deltaTime) {
                this->getDataFromEmitter();
                curDeltaTime = 0.0f;
            }
        }
            break;
        case State_Pause:{
            //游戏暂停状态,单机可以进行暂停,对战可以选择此功能
            //添加暂停界面
            _proceedview->addPausePanel();
            _proceedview->addPauseShade();
        }
            break;
        case State_End:{
            //游戏结束,弹出结果,分数.上传分数
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
        if (obj->getObjType() == Object_Role) continue;
        if (obj->getObjType() == Object_Block){
            Barrier* bar = dynamic_cast<Barrier*>(obj);
            bar->barrierAction();
        }
            
        obj->setBodyPosition(ccp(obj->getBodyPosition().x - obj->getSpeed() * dt, obj->getBodyPosition().y));
        
        //如果是背景对象而且位置已经超过了就删除并且添加新的
        if (obj->getObjType() == Object_Background &&
            obj->getBodyPosition().x <= -obj->getContentSize().width) {
            CCLOG("++++++++++remove background+++++++++++++++");
            
            GameObject *lastObj = this->popObjectFromType(Object_Background);
            
            _proceedview->drawBackground(ccp(lastObj->getBodyPosition().x + lastObj->getContentSize().width * lastObj->getScaleX(),
                                             lastObj->getBodyPosition().y));

            _proceedview->removeChild(obj);
            _proceedview->destroyBody(obj);
            this->deleteObject(obj);

            continue;
        }
        
        //陆地地面
        if (obj->getObjType() == Object_Land &&
            obj->getBodyPosition().x <= -obj->getContentSize().width) {
            CCLOG("+++++++++++++remove land++++++++++++++++++++++");
            
            GameObject *lastObj = this->popObjectFromType(Object_Land);
            
            _proceedview->drawLand(ccp(lastObj->getBodyPosition().x + lastObj->getContentSize().width * lastObj->getScaleX(), lastObj->getBodyPosition().y));
            _proceedview->removeChild(obj);
            _proceedview->destroyBody(obj);
            this->deleteObject(obj);
            
            continue;
        }
        
        //如果是非背景和非主角
        if (obj->getBodyPosition().x <= -obj->getContentSize().width) {
            CCLOG("++++++++++++++++remove game object++++++++++++++++");
            _proceedview->removeChild(obj);
            _proceedview->destroyBody(obj);
            this->deleteObject(obj);
        }
    }
}

void GameLogic::getDataFromEmitter(){
    EmitterMsg msg = _emitter->popData();
    CCLOG("+======Emitter:TYPE %d ++ POSITIONY %f=========+",msg.type,msg.positionY);
    switch (msg.type) {
        case Barrier_Stone:
            _proceedview->drawBarrier(Barrier_Stone, ccp(EMITTER_START_POSX,msg.positionY));
            break;
        case Barrier_Step:
            _proceedview->drawBarrier(Barrier_Step, ccp(EMITTER_START_POSX,msg.positionY));
            break;
        case Barrier_Gear:
            _proceedview->drawBarrier(Barrier_Gear,ccp(EMITTER_START_POSX,msg.positionY));
            break;
        case Barrier_Stab:
            _proceedview->drawBarrier(Barrier_Stab,ccp(EMITTER_START_POSX,SUB_HEIGHT + 40));
            break;
        case Barrier_Rocket:
            _proceedview->drawBarrier(Barrier_Rocket,ccp(EMITTER_START_POSX,msg.positionY));
            break;
        case Prop_Sprint:
            _proceedview->drawProp(Prop_Sprint,ccp(EMITTER_START_POSX,msg.positionY));
            break;
        case Prop_Blood:
            _proceedview->drawProp(Prop_Blood, ccp(EMITTER_START_POSX,msg.positionY));
            break;
        case Prop_Wave:
            _proceedview->drawProp(Prop_Wave, ccp(EMITTER_START_POSX, msg.positionY));
            break;
        default:
            break;
    }
}

void GameLogic::updateScore(){
    _score += _multiple;
}

void GameLogic::updateBlood(){
    if (_blood < 100) {
        _blood += 0.05;//如果小于100就不断的增加
    }
    if (_blood >= 100) {
        return;
    }
    _proceedview->updateBlood(_blood);
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
    exit(EXIT_FAILURE);
    
    return NULL;
}

void GameLogic::collisionListener(){
    if (_objects && _objects->count() > 0) {
        for (int i = 0; i < _objects->count(); i++) {
            GameObject* arr_obj = dynamic_cast<GameObject*>(_objects->objectAtIndex(i));
            
            if (arr_obj->getIsTrigger()) {//是否为触发者
                for (int j = 0; j < _objects->count(); j++) {
                    
                    GameObject* unRoleObj = dynamic_cast<GameObject*>(_objects->objectAtIndex(j));
                    
                    if (unRoleObj->getObjType() != Object_Role &&
                        unRoleObj->getObjType() != Object_Background) {//道具或者障碍物
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