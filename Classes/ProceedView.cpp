//
//  ProceedView.cpp
//  BlackWhiteRush
//
//  Created by 何遵祖 on 13-12-29.
//
//

#include "ProceedView.h"

#define PTM_RADIO 32

ProceedView* ProceedView::_singletonView = NULL;

ProceedView* ProceedView::Singleton(){
    if (!_singletonView) {
        CCLOG("+=============PROCEEDVIEW SINGLETON==================+");
        _singletonView = ProceedView::create();
    }
    return _singletonView;
}

ProceedView::~ProceedView(){
    CC_SAFE_DELETE(_world);
#if GLESDEBUG_DRAW_ENABLE
    CC_SAFE_DELETE(_debugDraw);
#endif
}

bool ProceedView::init(){
    if (!CCLayer::init()) {
        return false;
    }
    
    this->initVisibleSize();
    this->initGameSize();
    this->initPhysics();
    
    //做初始渲染
#if GLESDEBUG_DRAW_ENABLE == 0
    for (int i = 0; i < 2; i++) {
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        this->drawBackground(ccp(winSize.width/2 + winSize.width * i,winSize.height/2));
        //this->drawGrass(ccp(winSize.width/2 + winSize.width * i,320));
    }
#endif
    //主角
    Role *role = Role::instance("Icon-72.png");
    role->setPosition(ccp(100, 300));
    role->setBodyType(b2_dynamicBody);
    role->setDensity(1.0f);
    role->setFriction(0.3f);
    this->addChild(role);
    this->createBodyRect(role);
    GameLogic::Singleton()->pushObject(role);
    
    //测试===============
//    Props *prop = Props::instance("Icon-72.png");
//    prop->setPosition(ccp(960, 50));
//    prop->setBodyType(b2_staticBody);
//    prop->setDensity(1.0f);
//    prop->setFriction(0.3f);
//    this->addChild(prop);
//    this->createBodyRect(prop);
//    GameLogic::Singleton()->pushObject(prop);
    //测试===============
    
    return true;
}

void ProceedView::createBodyRect(GameObject *obj){
    b2BodyDef bodyDef;
    bodyDef.type = obj->getBodyType();
    bodyDef.position.Set(obj->getPosition().x/PTM_RADIO,obj->getPosition().y/PTM_RADIO);
    
    b2Body *body = _world->CreateBody(&bodyDef);
    
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(.5f, .5f);
    
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = obj->getDensity();
    fixtureDef.friction = obj->getFriction();
    body->CreateFixture(&fixtureDef);
    
    obj->setB2Body(body);
    obj->setPtmRadio(PTM_RADIO);
}

void ProceedView::initPhysics(){
    b2Vec2 gravity;
    gravity.Set(0.0f, -10.0f);
    
    _world = new b2World(gravity);
    
    _world->SetAllowSleeping(true);
    _world->SetContinuousPhysics(true);
    
#if GLESDEBUG_DRAW_ENABLE
    _debugDraw = new GLESDebugDraw(PTM_RADIO);
    _world->SetDebugDraw(_debugDraw);

    uint32 flags = 0;
    flags += b2Draw::e_shapeBit;
    flags += b2Draw::e_jointBit;
    flags += b2Draw::e_aabbBit;
    flags += b2Draw::e_pairBit;
    flags += b2Draw::e_centerOfMassBit;

    _debugDraw->SetFlags(flags);
#endif
    
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0, 0);
 
    b2Body* groundBody = _world->CreateBody(&groundBodyDef);
    b2EdgeShape groundBox;
    
    //bottom
    groundBox.Set(b2Vec2(_gSize.leftBottom.x/PTM_RADIO, _gSize.leftBottom.y/PTM_RADIO),
                  b2Vec2(_gSize.rightBottom.x/PTM_RADIO,_gSize.rightBottom.y/PTM_RADIO));
    groundBody->CreateFixture(&groundBox,0);
    //top
    groundBox.Set(b2Vec2(_gSize.leftTop.x/PTM_RADIO,_gSize.leftTop.y/PTM_RADIO),
                  b2Vec2(_gSize.rightTop.x/PTM_RADIO,_gSize.rightTop.y/PTM_RADIO));
    groundBody->CreateFixture(&groundBox,0);
    //left
    groundBox.Set(b2Vec2(_gSize.leftBottom.x/PTM_RADIO,_gSize.leftBottom.y/PTM_RADIO),
                  b2Vec2(_gSize.leftTop.x/PTM_RADIO,_gSize.leftTop.y/PTM_RADIO));
    groundBody->CreateFixture(&groundBox,0);
    //right
    groundBox.Set(b2Vec2(_gSize.rightBottom.x/PTM_RADIO,_gSize.rightBottom.y/PTM_RADIO),
                  b2Vec2(_gSize.rightTop.x/PTM_RADIO,_gSize.rightTop.y/PTM_RADIO));
    groundBody->CreateFixture(&groundBox,0);
}

void ProceedView::update(float dt){
    int velocityIterations = 8;
    int positionIterations = 1;
    
    _world->Step(dt, velocityIterations, positionIterations);
}

void ProceedView::initVisibleSize(){
    if (_visibleSize.size.width == 0.0f && _visibleSize.size.height == 0.0f) {
        CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();
        _visibleSize.origin = pEGLView->getVisibleOrigin();
        _visibleSize.size = pEGLView->getVisibleSize();
    }
    
    CCLOG("+====visible size=====+ x:%.2f,y%.2f,width%.2f,height%.2f",
          _visibleSize.origin.x,_visibleSize.origin.y,
          _visibleSize.size.width,_visibleSize.size.height);
}

void ProceedView::initGameSize(){
    _gSize.leftTop = ccp(_visibleSize.origin.x,_visibleSize.origin.y + _visibleSize.size.height);
    _gSize.leftBottom = ccp(_visibleSize.origin.x,_visibleSize.origin.y + SUB_HEIGHT);
    _gSize.rightTop = ccp(_visibleSize.origin.x + _visibleSize.size.width,
                          _visibleSize.origin.y + _visibleSize.size.height);
    _gSize.rightBottom = ccp(_visibleSize.origin.x + _visibleSize.size.width,
                             _visibleSize.origin.y + SUB_HEIGHT );
    CCLOG("+===game==size====leftTop(%f,%f)===leftBottom(%f,%f)===rightTop(%f,%f)==rightBottom(%f,%f)++",
          _gSize.leftTop.x,_gSize.leftTop.y,_gSize.leftBottom.x,_gSize.leftBottom.y,
          _gSize.rightTop.x,_gSize.rightTop.y,_gSize.rightBottom.x,_gSize.rightBottom.y);
}

void ProceedView::drawView(){
    
}

void ProceedView::drawBackground(cocos2d::CCPoint pos){
    BackDrop* backdrop = BackDrop::instance("bg_w.png");
    backdrop->setPosition(pos);
    backdrop->setBodyType(b2_staticBody);
    backdrop->setScale(2.0f);
    this->addChild(backdrop,-1);
    this->createBodyRect(backdrop);
    GameLogic::Singleton()->pushObject(backdrop);
}

void ProceedView::drawGrass(cocos2d::CCPoint pos){
    Grass* grass = Grass::instance("grass.png");
    grass->setPosition(pos);
    grass->setBodyType(b2_staticBody);
    grass->setScale(1.0f);
    this->addChild(grass,1);
    this->createBodyRect(grass);
    GameLogic::Singleton()->pushObject(grass);
}

void ProceedView::drawProp(cocos2d::CCPoint pos){
    Props* prop = Props::instance("Icon-72.png");
    prop->setPosition(ccp(960, 320));
    prop->setBodyType(b2_staticBody);
    prop->setDensity(1.0f);
    prop->setFriction(0.3f);
    this->addChild(prop);
    this->createBodyRect(prop);
    GameLogic::Singleton()->pushObject(prop);
}

void ProceedView::drawBarrier(cocos2d::CCPoint pos){
    Barrier *barrier = Barrier::instance("Icon-72.png");
    barrier->setPosition(ccp(960, 100));
    barrier->setBodyType(b2_staticBody);
    barrier->setDensity(1.0f);
    barrier->setFriction(0.3f);
    this->addChild(barrier);
    this->createBodyRect(barrier);
    GameLogic::Singleton()->pushObject(barrier);
}

#if GLESDEBUG_DRAW_ENABLE
void ProceedView::draw(){
    CCLayer::draw();
    
    ccGLEnableVertexAttribs( kCCVertexAttribFlag_Position );
    
    kmGLPushMatrix();
    
    _world->DrawDebugData();
    
    kmGLPopMatrix();
    
}
#endif

#pragma mark -
#pragma mark - GLESDEBUG

#if GLESDEBUG_DRAW_ENABLE
GLESDebugDraw::GLESDebugDraw()
: mRatio( 1.0f )
{
    this->initShader();
}

GLESDebugDraw::GLESDebugDraw( float32 ratio )
: mRatio( ratio )
{
    this->initShader();
}

void GLESDebugDraw::initShader( void )
{
    mShaderProgram = CCShaderCache::sharedShaderCache()->programForKey(kCCShader_Position_uColor);
    
    mColorLocation = glGetUniformLocation( mShaderProgram->getProgram(), "u_color");
}

void GLESDebugDraw::DrawPolygon(const b2Vec2* old_vertices, int vertexCount, const b2Color& color)
{
    mShaderProgram->use();
    mShaderProgram->setUniformsForBuiltins();
    
    b2Vec2* vertices = new b2Vec2[vertexCount];
    for( int i=0;i<vertexCount;i++)
    {
        vertices[i] = old_vertices[i];
        vertices[i] *= mRatio;
    }
    
    mShaderProgram->setUniformLocationWith4f(mColorLocation, color.r, color.g, color.b, 1);
    
    glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, vertices);
    glDrawArrays(GL_LINE_LOOP, 0, vertexCount);
    
    CC_INCREMENT_GL_DRAWS(1);
    
    CHECK_GL_ERROR_DEBUG();
    
    delete[] vertices;
}

void GLESDebugDraw::DrawSolidPolygon(const b2Vec2* old_vertices, int vertexCount, const b2Color& color)
{
    mShaderProgram->use();
    mShaderProgram->setUniformsForBuiltins();
    
    b2Vec2* vertices = new b2Vec2[vertexCount];
    for( int i=0;i<vertexCount;i++) {
        vertices[i] = old_vertices[i];
        vertices[i] *= mRatio;
    }
    
    mShaderProgram->setUniformLocationWith4f(mColorLocation, color.r*0.5f, color.g*0.5f, color.b*0.5f, 0.5f);
    
    glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, vertices);
    
    glDrawArrays(GL_TRIANGLE_FAN, 0, vertexCount);
    
    mShaderProgram->setUniformLocationWith4f(mColorLocation, color.r, color.g, color.b, 1);
    glDrawArrays(GL_LINE_LOOP, 0, vertexCount);
    
    CC_INCREMENT_GL_DRAWS(2);
    
    CHECK_GL_ERROR_DEBUG();
    
    delete[] vertices;
}

void GLESDebugDraw::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color)
{
    mShaderProgram->use();
    mShaderProgram->setUniformsForBuiltins();
    
    const float32 k_segments = 16.0f;
    int vertexCount=16;
    const float32 k_increment = 2.0f * b2_pi / k_segments;
    float32 theta = 0.0f;
    
    GLfloat*    glVertices = new GLfloat[vertexCount*2];
    for (int i = 0; i < k_segments; ++i)
    {
        b2Vec2 v = center + radius * b2Vec2(cosf(theta), sinf(theta));
        glVertices[i*2]=v.x * mRatio;
        glVertices[i*2+1]=v.y * mRatio;
        theta += k_increment;
    }
    
    mShaderProgram->setUniformLocationWith4f(mColorLocation, color.r, color.g, color.b, 1);
    glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, glVertices);
    
    glDrawArrays(GL_LINE_LOOP, 0, vertexCount);
    
    CC_INCREMENT_GL_DRAWS(1);
    
    CHECK_GL_ERROR_DEBUG();
    
    delete[] glVertices;
}

void GLESDebugDraw::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color)
{
    mShaderProgram->use();
    mShaderProgram->setUniformsForBuiltins();
    
    const float32 k_segments = 16.0f;
    int vertexCount=16;
    const float32 k_increment = 2.0f * b2_pi / k_segments;
    float32 theta = 0.0f;
    
    GLfloat*    glVertices = new GLfloat[vertexCount*2];
    for (int i = 0; i < k_segments; ++i)
    {
        b2Vec2 v = center + radius * b2Vec2(cosf(theta), sinf(theta));
        glVertices[i*2]=v.x * mRatio;
        glVertices[i*2+1]=v.y * mRatio;
        theta += k_increment;
    }
    
    mShaderProgram->setUniformLocationWith4f(mColorLocation, color.r*0.5f, color.g*0.5f, color.b*0.5f, 0.5f);
    glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, glVertices);
    glDrawArrays(GL_TRIANGLE_FAN, 0, vertexCount);
    
    
    mShaderProgram->setUniformLocationWith4f(mColorLocation, color.r, color.g, color.b, 1);
    glDrawArrays(GL_LINE_LOOP, 0, vertexCount);
    
    // Draw the axis line
    DrawSegment(center,center+radius*axis,color);
    
    CC_INCREMENT_GL_DRAWS(2);
    
    CHECK_GL_ERROR_DEBUG();
    
    delete[] glVertices;
}

void GLESDebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
    mShaderProgram->use();
    mShaderProgram->setUniformsForBuiltins();
    
    mShaderProgram->setUniformLocationWith4f(mColorLocation, color.r, color.g, color.b, 1);
    
    GLfloat    glVertices[] =
    {
        p1.x * mRatio, p1.y * mRatio,
        p2.x * mRatio, p2.y * mRatio
    };
    glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, glVertices);
    
    glDrawArrays(GL_LINES, 0, 2);
    
    CC_INCREMENT_GL_DRAWS(1);
    
    CHECK_GL_ERROR_DEBUG();
}

void GLESDebugDraw::DrawTransform(const b2Transform& xf)
{
    b2Vec2 p1 = xf.p, p2;
    const float32 k_axisScale = 0.4f;
    p2 = p1 + k_axisScale * xf.q.GetXAxis();
    DrawSegment(p1, p2, b2Color(1,0,0));
    
    p2 = p1 + k_axisScale * xf.q.GetYAxis();
    DrawSegment(p1,p2,b2Color(0,1,0));
}

void GLESDebugDraw::DrawPoint(const b2Vec2& p, float32 size, const b2Color& color)
{
    mShaderProgram->use();
    mShaderProgram->setUniformsForBuiltins();
    
    mShaderProgram->setUniformLocationWith4f(mColorLocation, color.r, color.g, color.b, 1);
    
    //    glPointSize(size);
    
    GLfloat                glVertices[] = {
        p.x * mRatio, p.y * mRatio
    };
    
    glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, glVertices);
    
    glDrawArrays(GL_POINTS, 0, 1);
    //    glPointSize(1.0f);
    
    CC_INCREMENT_GL_DRAWS(1);
    
    CHECK_GL_ERROR_DEBUG();
}

void GLESDebugDraw::DrawString(int x, int y, const char *string, ...)
{
    //    NSLog(@"DrawString: unsupported: %s", string);
    //printf(string);
    /* Unsupported as yet. Could replace with bitmap font renderer at a later date */
}

void GLESDebugDraw::DrawAABB(b2AABB* aabb, const b2Color& color)
{
    mShaderProgram->use();
    mShaderProgram->setUniformsForBuiltins();
    
    mShaderProgram->setUniformLocationWith4f(mColorLocation, color.r, color.g, color.b, 1);
    
    GLfloat                glVertices[] = {
        aabb->lowerBound.x * mRatio, aabb->lowerBound.y * mRatio,
        aabb->upperBound.x * mRatio, aabb->lowerBound.y * mRatio,
        aabb->upperBound.x * mRatio, aabb->upperBound.y * mRatio,
        aabb->lowerBound.x * mRatio, aabb->upperBound.y * mRatio
    };
    
    glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, glVertices);
    glDrawArrays(GL_LINE_LOOP, 0, 8);
    
    CC_INCREMENT_GL_DRAWS(1);
    
    CHECK_GL_ERROR_DEBUG();
    
}
#endif