//
//  PropBox.h
//  BlackWhiteRush
//
//  Created by 何遵祖 on 14-1-20.
//
//

#ifndef __BlackWhiteRush__PropBox__
#define __BlackWhiteRush__PropBox__

#include "cocos2d.h"
#include "GameObject.h"

USING_NS_CC;

class PropBox : public CCSprite , public CCStandardTouchDelegate{
public:
    static PropBox* instance(const char* file,int index);
    bool inits(const char* file,int index);
    
    PropType getType() const;
    void drawElementFromType(PropType ty);
    void renderObjFromType(PropType ty);
    
    bool pointInRect(CCPoint pos);

    virtual void onEnter();
    virtual void onExit();
    
    virtual void ccTouchesBegan(CCSet* pTouches,CCEvent* pEvent);
    virtual void ccTouchesMoved(CCSet* pTouches,CCEvent* pEvent);
    virtual void ccTouchesEnded(CCSet* pTouches,CCEvent* pEvent);
private:
    PropType _propType;
    
    CC_SYNTHESIZE_READONLY(int, _index, Index);//工具栏序列编号
};

#endif /* defined(__BlackWhiteRush__PropBox__) */
