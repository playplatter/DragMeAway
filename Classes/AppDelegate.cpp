#include "AppDelegate.h"
#include "MainMenuScene.h"

USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}



static cocos2d::Size designResolutionSize = cocos2d::Size(480, 320);


//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}        

bool AppDelegate::applicationDidFinishLaunching() {
    
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLViewImpl::create("My Game");
        director->setOpenGLView(glview);
    }
    
    // turn off display FPS
    director->setDisplayStats(false);
    
    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);
    
    auto screenSize = glview->getFrameSize();
    
    auto fileUtils = FileUtils::getInstance();
    std::vector<std::string> searchPaths;
    
    searchPaths.push_back("audios");
    searchPaths.push_back("actors");
    searchPaths.push_back("environment");
    searchPaths.push_back("powerups");
    glview -> setDesignResolutionSize(480, 320, ResolutionPolicy::FIXED_WIDTH);
    
    
//    if (screenSize.width == 2048 || screenSize.height == 2048)
//    {
//        glview -> setDesignResolutionSize(1536, 2048, ResolutionPolicy::NO_BORDER);
//        searchPaths.push_back("ipadhd");
//        searchPaths.push_back("ipadsd");
//        searchPaths.push_back("iphone5");
//        searchPaths.push_back("iphonehd");
//        searchPaths.push_back("iphonesd");
//    }
//    else if (screenSize.width == 1024 || screenSize.height == 1024)
//    {
//        glview -> setDesignResolutionSize(768, 1024, ResolutionPolicy::NO_BORDER);
//        searchPaths.push_back("ipadsd");
//        searchPaths.push_back("iphone5");
//        searchPaths.push_back("iphonehd");
//        searchPaths.push_back("iphonesd");
//        
//    }
//    else if (screenSize.width == 1136 || screenSize.height == 1136)
//    {
//        glview -> setDesignResolutionSize(640, 1136, ResolutionPolicy::NO_BORDER);
//        searchPaths.push_back("iphone5");
//        searchPaths.push_back("iphonehd");
//        searchPaths.push_back("iphonesd");
//        
//    }
//    else if (screenSize.width == 960 || screenSize.height == 960)
//    {
//        glview -> setDesignResolutionSize(640, 960, ResolutionPolicy::NO_BORDER);
//        searchPaths.push_back("iphonehd");
//        searchPaths.push_back("iphonesd");
//    }
//    else
//    {
//        searchPaths.push_back("iphonesd");
//        glview -> setDesignResolutionSize(320, 480, ResolutionPolicy::NO_BORDER);
//    }
    
    fileUtils->setSearchPaths(searchPaths);
    
    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
    auto scene =  MainMenu::createScene();

    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
