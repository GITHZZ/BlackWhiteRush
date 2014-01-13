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

#define ROWS 5
#define COLS 5

using namespace cocos2d;

class GameEmitter : public CCObject{
public:
    GameEmitter();
    ~GameEmitter();
    
    void updateDatas();
    
    void pushData(int data);
    int popData() const;
private:
    std::vector<int> datas;
    int *dataArr;
};

#endif /* defined(__BlackWhiteRush__GameEmitter__) */
