#include "AppDelegate.h"

#include <vector>
#include <string>

#include "AppMacros.h"
#include "MainSceneGame.h"
#include "Init/DisplayView.h"
#if USE_CC_AUDIO
#include "Utility/SoundManager.h"
#endif
USING_NS_CC;
using namespace std;
float scaleFactorTouch = 1.0f;
AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {
	// initialize director
	auto director = Director::getInstance();
	auto glview = director->getOpenGLView();
	if (!glview) {
#ifdef WP8
		glview = GLViewImpl::create("Jumping Bunny FlowerFX");
#else
		glview = GLViewImpl::createWithRect("Jumping Bunny FlowerFX", Rect(0, 0, ScreenSize.width, ScreenSize.height), 1);
#endif
	}

	director->setOpenGLView(glview);
#if defined WP8 || defined W8
	designResolutionSize = glview->getDesignResolutionSize();
	SizeResource.size = designResolutionSize;

	//scaleFactorTouch = designResolutionSize.height / 480;
#endif
    // Set the design resolution
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
    // a bug in DirectX 11 level9-x on the device prevents ResolutionPolicy::NO_BORDER from working correctly
    glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::SHOW_ALL);
#else
    glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::NO_BORDER);
#endif

	Size frameSize = glview->getFrameSize();
    
    vector<string> searchPath;

    // In this demo, we select resource according to the frame's height.
    // If the resource size is different from design resolution size, you need to set contentScaleFactor.
    // We use the ratio of resource's height to the height of design resolution,
    // this can make sure that the resource's height could fit for the height of design resolution.

	searchPath.push_back(SizeResource.directory);
        
	director->setContentScaleFactor(MIN(SizeResource.size.height / designResolutionSize.height, SizeResource.size.width / designResolutionSize.width));   
    // set searching path
    FileUtils::getInstance()->setSearchPaths(searchPath);
#ifdef _DEBUG
    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);
#endif
    // run
	MainSceneGame::GetInstance()->InitForFirstScence(director);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
#if USE_CC_AUDIO
	GetSound->PauseSoundEngine();
#endif
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
#if USE_CC_AUDIO
	GetSound->ResumeSoundEngine();
#endif
}
