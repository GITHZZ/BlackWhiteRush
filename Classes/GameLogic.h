//
//  GameLogic.h
//  BlackWhiteRush
//
//  Created by 何遵祖 on 13-12-27.
//
//

#ifndef __BlackWhiteRush__GameLogic__
#define __BlackWhiteRush__GameLogic__

#include "cocos2d.h"
#include "GameObject.h"
#include "GameEmitter.h"
#include "PropBox.h"

class ProceedView;//关联的游戏显示

USING_NS_CC;

#define LOG_GAMESTATE_ENABLED 0
#define UPDATE_INTERVAL 1.0f/60.0f
#define BACKGROUND_SPEED 100

//游戏模式(单人,多人)
enum GameModel {
    Model_Single,
    Model_Multiple
};

enum GameState {
    State_Playing, //进行
    State_Pause,   //暂停
    State_Start,   //开始
    State_End      //结束
};

enum FeverState{
    Fever_unable = 0,
    Fever_enable = 1,
    Fever_doing = 2
};

class GameLogic : public CCObject{
public:
    static GameLogic* Singleton();//单例
    ~GameLogic();
    void end();
    void init();
    void update(float dt);
    
    /*队列*/
    void pushObject(GameObject* gameObject);
    void deleteObject(GameObject* gameObject);
    GameObject* popObjectFromType(ObjectType t);
    
    //输出游戏状态
    void logGameState(GameState s);
    //更新分数
    void updateScore();
    //血量的更新
    void updateBlood();
    //更新游戏等级
    void updateLevel();
    //遍历对象方法
    void objectsActionFromTraverse(float dt);
    //移动对象
    void moveGameObject(float dt,int index);
    //碰撞检测
    void collisionListener(int index);
    //从发射器中获取数据并且通知View进行渲染
    void getDataFromEmitter();
    //改变游戏状态为结束
    void gameEnd();
private:
    static GameLogic* _singletonObject;
    
    GameEmitter* _emitter;
    CCScheduler* _pScheduler;
    
    float deltaTime;
    float curDeltaTime;
    int reciprocalTime;//时间倒数
    
    //游戏时间
    CC_SYNTHESIZE(float, _gameTime, GameTime);
    
    //游戏目前等级
    CC_SYNTHESIZE_READONLY(int, _level, Level);
    
    CC_SYNTHESIZE_READONLY(CCArray*, _objects, Objects);
    CC_SYNTHESIZE(ProceedView*, _proceedview, PView);
    
    //游戏模式和状态
    CC_SYNTHESIZE(GameModel, _currentModel, Model);
    CC_SYNTHESIZE(GameState, _state, State);
    
    //分数以及分数的倍数
    CC_SYNTHESIZE(int, _score, Score);
    CC_SYNTHESIZE(int, _multiple, Multiple);
    
    //分数倍数状态
    CC_SYNTHESIZE(FeverState,_feverState, FeverState);
    
    //血量
    CC_SYNTHESIZE(float, _blood, Blood);
};

#endif /* defined(__BlackWhiteRush__GameLogic__) */
