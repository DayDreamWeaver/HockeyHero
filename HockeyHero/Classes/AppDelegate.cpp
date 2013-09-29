//
//  HockeyHeroAppDelegate.cpp
//  HockeyHero
//
//  Created by hanks on 2013/09/28.
//  Copyright __MyCompanyName__ 2013年. All rights reserved.
//

#include "AppDelegate.h"

#include "cocos2d.h"
#include "layers/GameLayer.h"
#include "layers/LogoLayer.h"
#include "utils/SoundManager.h"
#include "HelloWorldScene.h"

USING_NS_CC;
using namespace CocosDenshion;

AppDelegate::AppDelegate()
{

}

AppDelegate::~AppDelegate()
{
}

bool AppDelegate::applicationDidFinishLaunching()
{
    // initialize director
    CCDirector *pDirector = CCDirector::sharedDirector();
    CCEGLView *pEGLView = CCEGLView::sharedOpenGLView();
    pDirector->setOpenGLView(pEGLView);
    
    // turn on display FPS
    pDirector->setDisplayStats(true);
    
    // get real screen size from device
    CCSize screenSize = pEGLView->getFrameSize();
    // set design size
    pEGLView->setDesignResolutionSize(DESIGN_RESOLUTION_WIDTH, DESIGN_RESOLUTION_HEIGHT, kResolutionShowAll);
    
    std::vector<std::string> searchPaths;
    if (screenSize.width > DESIGN_RESOLUTION_WIDTH) {
        printf("hd here.\n");
        searchPaths.push_back("hd");
        pDirector->setContentScaleFactor(screenSize.width / DESIGN_RESOLUTION_WIDTH);
    } else {
        printf("sd here.\n");
        searchPaths.push_back("sd");
        pDirector->setContentScaleFactor(screenSize.width / DESIGN_RESOLUTION_WIDTH);
    }
    CCFileUtils::sharedFileUtils()->setSearchResolutionsOrder(searchPaths);
    
    // init sound
    SoundManager::initSoundEnvironment();

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);

    // first show logo scene
    //CCScene *pScene = LogoLayer::scene();
    // create a scene. it's an autorelease object
    CCScene *pScene = HelloWorld::scene();

    // run
    pDirector->runWithScene(pScene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    CCDirector::sharedDirector()->stopAnimation();
    SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
    SimpleAudioEngine::sharedEngine()->pauseAllEffects();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    CCDirector::sharedDirector()->startAnimation();
    SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
    SimpleAudioEngine::sharedEngine()->resumeAllEffects();
}
