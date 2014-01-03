//
//  GameObject.h
//  BlackWhiteRush
//
//  Created by 何遵祖 on 13-12-28.
//
//

#ifndef __BlackWhiteRush__GameObject__
#define __BlackWhiteRush__GameObject__

#include "cocos2d.h"
#include "Box2D.h"

using namespace cocos2d;

enum ObjectType {
    Object_Role,//主角
    Object_Prop,//道具
    Object_Block,//障碍物
    Object_Background//背景
};

enum BarrierType {
    Barrier_Stone = 0,//石头
    Barrier_Step = 1,//台阶
    Barrier_Gear = 2,//齿轮
    Barrier_Stab = 3,//刺
    Barrier_Rocket = 4 //火箭
};

enum PropType {
    Prop_Sprint = 5, //冲刺
    Prop_Blood = 6, //补血
    Prop_Wave = 7   //冲击波
};

class GameObject : public CCSprite{
public:
    virtual bool init();
    virtual void onCollisionEnter(GameObject* collision) = 0;
    virtual CCAffineTransform nodeToParentTransform();
    
    void setB2Body(b2Body* body);
    b2Body* getB2Body() const;
    
    void setPtmRadio(float ptm);
    float getPtmRadio() const;
    
    void setBodyPosition(const CCPoint &position);
    CCPoint getBodyPosition() const;
    
    void checkCollision(GameObject* collision);
protected:
    //精灵对象物理Body
    b2Body* _pB2Body;
    //将单位像素转换成米的比例
    float _ptmRadio;
    //对象类型
    CC_SYNTHESIZE_READONLY(ObjectType,_objType,ObjType);
private:
    //是否为活跃状态
    CC_SYNTHESIZE(bool,isActive,IsActive);
    //物理属性
    CC_SYNTHESIZE(b2BodyType, _bType, BodyType);
    //摩擦力
    CC_SYNTHESIZE(float, _density, Density);
    //弹力
    CC_SYNTHESIZE(float, _friction, Friction);
};

#endif /* defined(__BlackWhiteRush__GameObject__) */
