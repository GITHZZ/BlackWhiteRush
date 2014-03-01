//
//  GameEmitter.cpp
//  BlackWhiteRush
//
//  Created by 何遵祖 on 14-1-12.
//
//

#include "GameEmitter.h"
#include "GameLogic.h"

//5个level
//行6列5
//石头:0 台阶:1 齿轮:2 刺:3 火箭:4
//冲刺:5 补血:6 冲击波:7
struct EmitterMsg objs[ROWS * COLS] = {
    //Level1
    {500.0f,2},{300.0f,2},{350.0f,7},{300.0f,2},{250.0f,2},
    {300.0f,5},{250.0f,2},{350.0f,2},{250.0f,2},{350.0f,2},
    {250.0f,3},{320.0f,6},{500.0f,2},{250.0f,2},{300.0f,2},
    
    //Level2
    {300.0f,2},{230.0f,2},{350.0f,2},{370.0f,2},{250.0f,2},
    {300.0f,2},{170.0f,2},{200.0f,2},{170.0f,2},{200.0f,2},
    {500.0f,2},{300.0f,2},{350.0f,7},{300.0f,2},{180.0f,2},
    {300.0f,2},{400.0f,2},{200.0f,2},{270.0f,2},{520.0f,2},
    {250.0f,3},{320.0f,6},{500.0f,2},{250.0f,2},{250.0f,3},
    
    //Level3
    {300.0f,2},{250.0f,7},{640.0f,0},{170.0f,2},{270.0f,4},
    {300.0f,4},{350.0f,4},{540.0f,4},{170.0f,4},{270.0f,4},
    
    //Level4
    
    //Level5
};

GameEmitter::GameEmitter(){    
    dataArr = objs;
    
    //初始化数据
    this->updateDatas();
}

GameEmitter::~GameEmitter(){
    //清空所有数据
    datas.clear();
}

void GameEmitter::updateDatas(){
    //控制游戏时间,5min以内 GameLogic->gametime /s
//    CCLOG("+=========G-T==============+%f",GameLogic::Singleton()->getGameTime());
    int curRow = arc4random() % 3;
//    switch (GameLogic::Singleton()->getLevel()) {
//        case 1:
//            curRow = arc4random() % 3;//0~2
//            break;
//        case 2:
//            curRow = arc4random() % 5 + 3;//3-7
//            break;
//        case 3:
//            curRow = arc4random() % 5;//0~4
//            break;
//        case 4:
//            curRow = arc4random() % 5;
//            break;
//        case 5:
//            curRow = arc4random() % 5;
//            break;
//        default:
//            break;
//    }

    CCLOG("++============GAME EMITTER=============+ INDEX:%d",curRow);
    //插入数据(随机抽取是那一列作为初始化)
    for (int i = 0; i < COLS; i++) {
        datas.push_back(dataArr[curRow * COLS + i]);
        //CCLOG("+======push data========+ %d",dataArr[curRow * COLS + i]);
    }
}   

void GameEmitter::pushData(EmitterMsg data){
    datas.push_back(data);
}

EmitterMsg GameEmitter::popData(){
    //取出队列的头节点
    EmitterMsg data;
    if (datas.size() <= 0) {
        data.positionY = 0.0f;
        data.type = -1;
    }else{
        if (datas.size() <= 3) {//生成新的数据
            this->updateDatas();
        }
        data = datas[0];
        datas.erase(datas.begin());
    }
    return data;
}