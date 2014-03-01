//
//  GameEmitter.h
//  BlackWhiteRush
//
//  Created by 何遵祖 on 14-1-12.
//

/*
    作为游戏中障碍物,地面道具的发射器
 */


#ifndef __BlackWhiteRush__GameEmitter__
#define __BlackWhiteRush__GameEmitter__

#include "cocos2d.h"

#define ROWS 10
#define COLS 5

struct EmitterMsg{
    float positionY;//发射的Y轴位置
    int type;//类型
};

USING_NS_CC;

class GameEmitter : public CCObject{
public:
    GameEmitter();
    ~GameEmitter();
    
    void updateDatas();
    
    void pushData(EmitterMsg data);
    EmitterMsg popData();
private:
    std::vector<EmitterMsg> datas;
    EmitterMsg* dataArr;
};

#endif /* defined(__BlackWhiteRush__GameEmitter__) */
