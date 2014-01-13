//
//  ProceedView.h
//  BlackWhiteRush
//
//  Created by 何遵祖 on 13-12-29.
//

/*
    游戏进行时渲染部分:(除去控制部分)
        
        主要是渲染,主角,背景,(非控制部分),分数渲染,用户不可控制部分,被逻辑所影响的部分
 
        涉及到box2D的也是放到此层中
    
        游戏中只有此层涉及到物理引擎方面，同时逻辑可以影响到它
 */

#ifndef __BlackWhiteRush__ProceedView__
#define __BlackWhiteRush__ProceedView__

#include "cocos2d.h"
#include "GameLogic.h"
#include "Role.h"
#include "Props.h"
#include "Land.h"
#include "Barrier.h"
#include "BackDrop.h"
#include "Box2D.h"

#define GLESDEBUG_DRAW_ENABLE 0
#define SUB_HEIGHT 200 //最低高度

#if GLESDEBUG_DRAW_ENABLE 
class GLESDebugDraw;
#endif

struct GameSize{
    CCPoint leftTop;
    CCPoint leftBottom;
    CCPoint rightTop;
    CCPoint rightBottom;
};

using namespace cocos2d;

class ProceedView : public CCLayer{
public:
    CREATE_FUNC(ProceedView);
    static ProceedView* Singleton();
    ~ProceedView();
    bool init();
    
    void update(float dt);
    
    void initPhysics();
    void initVisibleSize();
    void initGameSize();
    
    void createBodyRect(GameObject* obj);
    void destroyBody(GameObject* obj);
    
    void drawView();
    void drawBackground(CCPoint pos);
    void drawProp(PropType type,CCPoint pos);
    void drawBarrier(BarrierType type,CCPoint pos);
    void drawLand(CCPoint pos);
    
    void updateScore();
    void updateFever();
private:
    static ProceedView* _singletonView;
    
    b2World* _world;
    CCRect _visibleSize;
    GameSize _gSize;
    
    CCLabelAtlas* scoreStr;
    CCProgressTimer* feverProgress;
    
    CC_SYNTHESIZE_READONLY(Role*,role,Role);
    
#if GLESDEBUG_DRAW_ENABLE
    GLESDebugDraw* _debugDraw;
    void draw();
#endif
};

#if GLESDEBUG_DRAW_ENABLE
//Using In Box2D Debug
class GLESDebugDraw : public b2Draw
{
    float32 mRatio;
    cocos2d::CCGLProgram* mShaderProgram;
    GLint        mColorLocation;
    
    void initShader( void );
public:
    GLESDebugDraw();
    
    GLESDebugDraw( float32 ratio );
    
    virtual void DrawPolygon(const b2Vec2* vertices, int vertexCount, const b2Color& color);
    
    virtual void DrawSolidPolygon(const b2Vec2* vertices, int vertexCount, const b2Color& color);
    
    virtual void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color);
    
    virtual void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color);
    
    virtual void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color);
    
    virtual void DrawTransform(const b2Transform& xf);
    
    virtual void DrawPoint(const b2Vec2& p, float32 size, const b2Color& color);
    
    virtual void DrawString(int x, int y, const char* string, ...);
    
    virtual void DrawAABB(b2AABB* aabb, const b2Color& color);
};

#endif

#endif /* defined(__BlackWhiteRush__ProceedView__) */
