//
//  GameEmitter.cpp
//  BlackWhiteRush
//
//  Created by 何遵祖 on 14-1-12.
//
//

#include "GameEmitter.h"

//行5列5
//石头:0 台阶:1 齿轮:2 刺:3 火箭:4
//冲刺:5 补血:6 冲击波:7
struct EmitterMsg objs[ROWS * COLS] = {
    {500.0f,2},{300.0f,5},{350.0f,6},{350.0f,7},{180.0f,2},
    {300.0f,2},{170.0f,2},{200.0f,2},{170.0f,2},{200.0f,2},
    {300.0f,2},{170.0f,2},{200.0f,2},{170.0f,2},{200.0f,2},
    {300.0f,2},{170.0f,2},{200.0f,2},{170.0f,2},{200.0f,2},
    {300.0f,2},{170.0f,2},{200.0f,2},{170.0f,2},{200.0f,2}
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
//    int curRow = arc4random() % 5;//0~4
    int curRow = 0;
    //插入数据(随机抽取是那一列作为初始化)
    for (int i = 0; i < COLS; i++) {
        datas.push_back(dataArr[curRow * ROWS + i]);
        CCLOG("+======push data========+ %d",datas[i].type);
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
            //this->updateDatas();
        }
        data = datas[0];
        datas.erase(datas.begin());
    }
    return data;
}