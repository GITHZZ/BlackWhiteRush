//
//  ProceedView.cpp
//  BlackWhiteRush
//
//  Created by 何遵祖 on 13-12-29.
//
//

#include "ProceedView.h"
#include "ProceedPause.h"
#include "ProceedEnd.h"

#define PTM_RADIO 32

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
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
#if GLESDEBUG_DRAW_ENABLE == 0
    for (int i = 0; i < 5; i++) {
        CCSpriteFrame *bgFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("bg_w.png");
        this->drawBackground(ccp(winSize.width/2 + bgFrame->getRect().size.width * 2 * i,winSize.height/2));
    }
#endif
    //主角
    role = Role::instance("player.png");
    role->setPosition(role->getStandardPos());
    role->setBodyType(b2_dynamicBody);
    role->setDensity(1.0f);
    role->setScale(2.0f);
    role->setFriction(0.3f);
    this->addChild(role);
    this->createBodyRect(role);
    GameLogic::Singleton()->pushObject(role);
    
    //ferver Bg
    CCSpriteFrame* feverFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("Fever_1_2.png");
    CCSprite* feverBg = CCSprite::createWithSpriteFrame(feverFrame);
    
    feverProgress = CCProgressTimer::create(feverBg);
    feverProgress->setPercentage(0.0f);
    feverProgress->setType(kCCProgressTimerTypeRadial);
    feverProgress->setPosition(ccp(winSize.width - feverBg->getContentSize().width - 180, 100));
    this->addChild(feverProgress);
    
    feverBtn = CCSprite::createWithSpriteFrameName("Fever_1_1.png");
    feverBtn->setPosition(feverProgress->getPosition());
    feverBtn->setScale(1.5f);
    this->addChild(feverBtn,101);
    
    //分数
    scoreStr = CCLabelBMFont::create("000000", "ballpark-32-hd.fnt");
    scoreStr->setPosition(ccp(200,winSize.height - scoreStr->getContentSize().height ));
    this->addChild(scoreStr);
    
    //底部陆地
    for (int i = 0; i < 15; i++) {
        CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("land.png");
        this->drawLand(ccp(- frame->getRect().size.width + frame->getRect().size.width * i , SUB_HEIGHT));
    }
    
    //工具栏
    for (int i = 0; i < 3; i ++) {
        propBox[i] = PropBox::instance("PropBox.png",i);
        propBox[i]->setPosition(ccp(propBox[i]->getContentSize().width + propBox[i]->getContentSize().width *
                               propBox[i]->getScale() * i,100));
        
        this->addChild(propBox[i]);
    }
    
    //血条
    CCSprite* bloodBg = CCSprite::createWithSpriteFrameName("bloodBg.png");
    bloodBg->setPosition(ccp(scoreStr->getPosition().x + 400,
                             winSize.height - bloodBg->getContentSize().height/2 - 30));
    this->addChild(bloodBg);
    
    CCSprite* spriteFrame = CCSprite::createWithSpriteFrameName("blood.png");
    bloodProgress = CCProgressTimer::create(spriteFrame);
    bloodProgress->setType(kCCProgressTimerTypeBar);
    bloodProgress->setPosition(bloodBg->getPosition());
    bloodProgress->setMidpoint(ccp(0,0.5));
    bloodProgress->setBarChangeRate(ccp(1,0));
    bloodProgress->setPercentage(100.0f);
    
    this->addChild(bloodProgress);
    
    //倒数数字
    startReciprocal = CCLabelBMFont::create("3","ballpark-32-hd.fnt");
    startReciprocal->setPosition(ccp(winSize.width/2, winSize.height/2));
    startReciprocal->setScale(2.5f);
    this->addChild(startReciprocal);
    
    return true;
}

void ProceedView::createBodyRect(GameObject *obj){
    b2BodyDef bodyDef;
    bodyDef.type = obj->getBodyType();
    bodyDef.position.Set(obj->getPosition().x/PTM_RADIO,obj->getPosition().y/PTM_RADIO);

    b2Body *body = _world->CreateBody(&bodyDef);
    
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(obj->getBoxRect().width,obj->getBoxRect().height);
    
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = obj->getDensity();
    fixtureDef.friction = obj->getFriction();
    fixtureDef.filter.groupIndex = obj->getGroundIndex();
    body->CreateFixture(&fixtureDef);

    obj->setB2Body(body);
    obj->setPtmRadio(PTM_RADIO);
}

void ProceedView::destroyBody(GameObject *obj){
    if (!obj->getIsBody()) return;
    b2Body* objBody = obj->getB2Body();
    CCAssert(objBody, "Fail to destroy body because body is null");
    _world->DestroyBody(objBody);
}

void ProceedView::initPhysics(){
    b2Vec2 gravity;
    gravity.Set(0.0f, -16.0f);
    
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
    _gSize.leftBottom = ccp(_visibleSize.origin.x,_visibleSize.origin.y);
    _gSize.rightTop = ccp(_visibleSize.origin.x + _visibleSize.size.width,
                          _visibleSize.origin.y + _visibleSize.size.height);
    _gSize.rightBottom = ccp(_visibleSize.origin.x + _visibleSize.size.width,
                             _visibleSize.origin.y);
    CCLOG("+===game==size====leftTop(%f,%f)===leftBottom(%f,%f)===rightTop(%f,%f)==rightBottom(%f,%f)++",
          _gSize.leftTop.x,_gSize.leftTop.y,_gSize.leftBottom.x,_gSize.leftBottom.y,
          _gSize.rightTop.x,_gSize.rightTop.y,_gSize.rightBottom.x,_gSize.rightBottom.y);
}

void ProceedView::drawView(){
    //分数,血条,fever非按钮部分
    this->updateScore();
}

void ProceedView::updateScore(){
    int score = GameLogic::Singleton()->getScore();
    int multiple = GameLogic::Singleton()->getMultiple();
    
    CCString *scoreLabel;
    if (score >= 100000) {
        scoreLabel = CCString::createWithFormat("%d x%d",score,multiple);
    }else if(score >= 10000){
        scoreLabel = CCString::createWithFormat("0%d x%d",score,multiple);
    }else if(score >= 1000){
        scoreLabel = CCString::createWithFormat("00%d x%d",score,multiple);
    }else if(score >= 100){
        scoreLabel = CCString::createWithFormat("000%d x%d",score,multiple);
    }else if(score >= 10){
        scoreLabel = CCString::createWithFormat("0000%d x%d",score,multiple);
    }else if(score >= 0){
        scoreLabel = CCString::createWithFormat("00000%d x%d",score,multiple);
    }
    scoreStr->setString(scoreLabel->getCString());
}

void ProceedView::updateFever(float feverCount,FeverState state){
    feverProgress->setPercentage(feverCount);
    if (feverCount >= 100.0f) {
        int feverOpacity = feverBtn->getOpacity();
        if (feverOpacity <= 150) {
            feverBtn->setOpacity(255);
        }
        feverBtn->setOpacity(feverOpacity - 10.0f);
    }
    if (state == Fever_doing) {
        feverBtn->setOpacity(255);
        feverBtn->setRotation(feverBtn->getRotation() + 0.5f * (GameLogic::Singleton()->getMultiple() / 10));
    }
}

void ProceedView::updateBlood(float percentage){
    //CCProgressFromTo* action = CCProgressFromTo::create(0.5f,bloodProgress->getPercentage(),percentage);
    //bloodProgress->runAction(action);
    bloodProgress->setPercentage(percentage);
}

void ProceedView::addPausePanel(){
    CCNode* node = this->getChildByTag(PAUSE_PANEL_TAG);
    if (node != NULL) {
        return;
    }
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    ProceedPause* _proceedPause = ProceedPause::create();
    _proceedPause->setPosition(ccp(winSize.width/2, winSize.height/2));
    _proceedPause->setScale(0.0f);
    _proceedPause->runAction(CCScaleTo::create(0.2f, 1.0f));
    this->addChild(_proceedPause,101,PAUSE_PANEL_TAG);
}

void ProceedView::removePausePanel(){
    CCNode* node = this->getChildByTag(PAUSE_PANEL_TAG);
    if (node == NULL) {
        return;
    }
    this->removeChildByTag(PAUSE_PANEL_TAG, true);
}

void ProceedView::addEndPanel(){
    CCNode* node = this->getChildByTag(END_PANEL_TAG);
    if (node != NULL) {
        return;
    }
    
    //CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    ProceedEnd* pe = ProceedEnd::create();
    this->addChild(pe,102,END_PANEL_TAG);
}

void ProceedView::addPauseShade(){
    CCNode* node = this->getChildByTag(PAUSE_SHADE_TAG);
    if (node != NULL) {
        return;
    }
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCLayerColor* shade = CCLayerColor::create(ccc4(0, 0, 0, 150),winSize.width, winSize.height);
    this->addChild(shade,100,PAUSE_SHADE_TAG);
}

void ProceedView::removePauseShade(){
    CCNode* node = this->getChildByTag(PAUSE_SHADE_TAG);
    if (node == NULL) {
        return;
    }
    this->removeChildByTag(PAUSE_SHADE_TAG);
}

void ProceedView::drawBackground(cocos2d::CCPoint pos){
    BackDrop* backdrop = BackDrop::instance("bg_w.png");
    backdrop->setPosition(pos);
    backdrop->setScale(2.0f);
    this->addChild(backdrop,-1);
    GameLogic::Singleton()->pushObject(backdrop);
}

void ProceedView::drawLand(cocos2d::CCPoint pos){
    Land* land = Land::instance("land.png");
    land->setPosition(pos);
    land->setBodyType(b2_staticBody);
    land->setBoxRect(CCSizeMake(land->getContentSize().width/PTM_RADIO,
                                land->getContentSize().height/PTM_RADIO));
    land->setColor(ccBLACK);
    land->setScale(1.0f);
    this->addChild(land,1);
    this->createBodyRect(land);
    //GameLogic::Singleton()->pushObject(land);
}

void ProceedView::drawProp(PropType type,cocos2d::CCPoint pos){
    const char* propName = NULL;
    if (type == Prop_Sprint) propName = "s.png";
    if (type == Prop_Blood)  propName = "B.png";
    if (type == Prop_Wave)   propName = "w.png";
    
    CCAssert(propName != NULL, "fail to get barrier file Name!");
    
    Props* prop = Props::instance(propName);
    prop->setPosition(pos);
    prop->runAction(CCRepeatForever::create(CCSequence::create(CCScaleTo::create(0.5f,1.5f),CCScaleTo::create(0.5f,1.0f),NULL)));
    prop->setPropType(type);
    this->addChild(prop);
    GameLogic::Singleton()->pushObject(prop);
}

void ProceedView::drawBarrier(BarrierType type,cocos2d::CCPoint pos){
    const char* barrierName = NULL;
    if (type == Barrier_Gear)   barrierName = "gear1.png";
    if (type == Barrier_Step)   barrierName = "step1.png";
    if (type == Barrier_Stab)   barrierName = "stab.png";
    if (type == Barrier_Stone)  barrierName = "stone.png";
    if (type == Barrier_Rocket) barrierName = "triangle.png";
    
    CCAssert(barrierName != NULL, "fail to get barrier file Name!");
    
    Barrier* barrier = Barrier::instance(barrierName);
    if (type == Barrier_Rocket) {
        barrier->setSpeed(1000);
    }
    barrier->setPosition(pos);
    barrier->setBarrierType(type);
    if (type != Barrier_Rocket) barrier->setScale(1.5f);
    if (type == Barrier_Stone) {
        //barrier->setIsTrigger(true);
        barrier->setIsBody(true);
        barrier->setBodyType(b2_dynamicBody);
        barrier->setBoxRect(CCSizeMake(0.1f, 0.1f));
        barrier->setDensity(1.0f);
        barrier->setFriction(0.3f);
    }
    
    this->addChild(barrier);
    
    if (type == Barrier_Stone) this->createBodyRect(barrier);
    
    GameLogic::Singleton()->pushObject(barrier);
}

void ProceedView::drawPropToBox(PropType ty){
    for (int i = 0; i < 3; i++) {
        //不为空
        if (propBox[i]->getType() != Prop_None) {
            continue;
        }
        
        propBox[i]->drawElementFromType(ty);
        break;
    }
}

#if GLESDEBUG_DRAW_ENABLE
void ProceedView::draw(){
    CCLayer::draw();
    
    ccGLEnableVertexAttribs( kCCVertexAttribFlag_Position );
    
    kmGLPushMatrix();
    
    _world->DrawDebugData();
    
    this->drawCollisionBox();
    
    kmGLPopMatrix();
    
}

void ProceedView::drawCollisionBox(){
    CCArray* objects = GameLogic::Singleton()->getObjects();
    if (objects && objects->count() > 0) {
        for (int i = 0; i < objects->count(); i++) {
            GameObject* obj = dynamic_cast<GameObject*>(objects->objectAtIndex(i));
            ccDrawColor4B(0, 0, 255, 255);
            glLineWidth(3);
            ccDrawRect(ccp(obj->getObjPosition().x - obj->getContentSize().width/2 * (obj->getScale() - 0.5),
                           obj->getObjPosition().y - obj->getContentSize().height/2 * (obj->getScale() - 0.5)),
                       ccp(obj->getObjPosition().x + obj->getContentSize().width/2 * (obj->getScale() - 0.5),
                           obj->getObjPosition().y + obj->getContentSize().height/2 * (obj->getScale() - 0.5)));
        }
    }
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