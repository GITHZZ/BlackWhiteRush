#include "AppDelegate.h"
#include "HelloWorldScene.h"
#include "ProceedScene.h"

USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
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
    CCSize screenSize = CCEGLView::sharedOpenGLView()->getFrameSize();
    
    if (screenSize.height > 768) {
        float height = 768;
        float width = (height/screenSize.height) * screenSize.width;
        CCEGLView::sharedOpenGLView()->setDesignResolutionSize(width, height, kResolutionExactFit);
    }else if(screenSize.height < 640){
        float height = 640;
        float width = (height/screenSize.height) * screenSize.width;
        CCEGLView::sharedOpenGLView()->setDesignResolutionSize(width, height, kResolutionExactFit);
    }
    
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    CCLOG("origin size ++ width:%.2f,height:%.2f real size ++ width:%.2f,height%.2f",screenSize.width,screenSize.height,size.width,size.height);
    
    // create a scene. it's an autorelease object
    //CCScene *pScene = HelloWorld::scene();
    CCScene *pScene = ProceedScene::create();
    
    // run
    pDirector->runWithScene(pScene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    CCDirector::sharedDirector()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    CCDirector::sharedDirector()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
