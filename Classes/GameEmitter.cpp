//
//  GameEmitter.cpp
//  BlackWhiteRush
//
//  Created by 何遵祖 on 14-1-12.
//
//

#include "GameEmitter.h"

//行5列5
int objs[ROWS * COLS] = {
    1,1,1,1,1,
    2,2,2,2,2,
    3,3,3,3,3,
    4,4,4,4,4,
    5,5,5,5,5
};

GameEmitter::GameEmitter(){    
    dataArr = objs;
    
    int curRow = arc4random() % 5;//0~4
    
    //插入数据(随机抽取是那一列作为初始化)
    for (int i = 0; i < 5; i++) {
        datas.push_back(dataArr[curRow * ROWS + i]);
        CCLOG("+======push data========+ %d",datas[i]);
    }
}

GameEmitter::~GameEmitter(){
    
}

void GameEmitter::updateDatas(){
    
}

void GameEmitter::pushData(int data){
    datas.push_back(data);
}

int GameEmitter::popData() const{
    return datas[0];
}