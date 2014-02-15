#include "AppDelegate.h"
#include "SimpleAudioEngine.h"
#include "HelloWorldScene.h"
#include "ProceedScene.h"
#include "StartScene.h"

USING_NS_CC;
using namespace CocosDenshion;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
    SimpleAudioEngine::sharedEngine()->end();
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    CCDirector* pDirector = CCDirector::sharedDirector();
    CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();

    pDirector->setOpenGLView(pEGLView);
	
    // turn on display FPS
    pDirector->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);
    
    //适应不同的屏幕(主要适应Android和IOS)
    //默认支持的分辨率 960 × 640 3:2
    CCSize screenSize = CCEGLView::sharedOpenGLView()->getFrameSize();
    CCSize designSize;
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    //hd
    if (screenSize.height > 320) {
        designSize = CCSizeMake(960, 640);
    }else{
        designSize = CCSizeMake(480, 320);
    }
    CCEGLView::sharedOpenGLView()->setDesignResolutionSize(designSize.width, designSize.height, kResolutionShowAll);
#endif
    CCLOG("+++++++++Origin Size %f,%f+++++++REAL Size+++++++%f,%f",screenSize.width,screenSize.height,
          designSize.width,designSize.height);
    
    //create a scene. it's an autorelease object
    //CCScene *pScene = HelloWorld::scene();
    CCScene *pScene = ProceedScene::create();
    //CCScene* pScene = StartAnim::scene();
    //CCScene* pScene = StartScene::scene();
    
    // run
    pDirector->runWithScene(pScene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    CCDirector::sharedDirector()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    CCDirector::sharedDirector()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
