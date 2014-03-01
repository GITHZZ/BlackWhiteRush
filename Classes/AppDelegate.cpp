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
    //pDirector->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);
    
    //文件路径(跨平台)
    CCFileUtils* pFileUtils = CCFileUtils::sharedFileUtils();
    std::vector<std::string> searchPaths;
    
    searchPaths.push_back("GameMusic/Android");
    searchPaths.push_back("GameMusic/Ios");
    searchPaths.push_back("GameImage/Particles");
    searchPaths.push_back("GameImage");
    
    pFileUtils->setSearchPaths(searchPaths);
    
    //适应不同的屏幕(主要适应Android和IOS)
    //默认支持的分辨率 960 × 640 3:2
    CCSize screenSize = CCEGLView::sharedOpenGLView()->getFrameSize();
    //CCSize designSize;
    if (screenSize.height > 768) {
        float height = 768;
        float width = (height/screenSize.height) * screenSize.width;
        CCEGLView::sharedOpenGLView()->setDesignResolutionSize(width, height, kResolutionExactFit);
    }else if(screenSize.height < 640){
        float height = 640;
        float width = (height/screenSize.height) * screenSize.width;
        CCEGLView::sharedOpenGLView()->setDesignResolutionSize(width, height, kResolutionExactFit);
    }
    
    //CCSize size = CCDirector::sharedDirector()->getWinSize();
    //CCLOG("+++++++++Origin Size %f,%f+++++++REAL Size+++++++%f,%f",screenSize.width,screenSize.height,
    //          size.width,size.height);
    
    //create a scene. it's an autorelease object
    CCScene *pScene = HelloWorld::scene();
    //CCScene *pScene = ProceedScene::create();
    //CCScene* pScene = StartAnim::scene();
    //CCScene* pScene = StartScene::scene();
    
    // run
    pDirector->runWithScene(pScene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    CCDirector::sharedDirector()->stopAnimation();
    SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    CCDirector::sharedDirector()->startAnimation();
    SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
    
    if (GameLogic::Singleton()->getState() == State_Playing) {
        GameLogic::Singleton()->setState(State_Pause);
    }
}
