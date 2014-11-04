//
//  SplashScreem.cpp
//  DragMeAway
//
//  Created by Syeda Sara Furqan on 23/10/2014.
//
//

#include "SplashScreen.h"
#include "MainMenuScene.h"



Scene* Splash::createScene() {
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = Splash::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}


bool Splash::init()
{
    // 1. super init first
    if( !LayerColor::initWithColor(Color4B(255,255,255,255)) ){
        return false;
    }
    
    
    Size visibleSize = CCDirector::getInstance()->getWinSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    
    // add static background image
    auto logo = Sprite::create("logo.png");
    
    // position the sprite on the center of the screen
    logo->setPosition(
                     Vec2(origin.x + visibleSize.width / 2,
                          origin.y + visibleSize.height / 2));
    
    // calculate the scaling factor to fill the window size
    float bX = visibleSize.width / logo->getContentSize().width;
    float bY = visibleSize.height / logo->getContentSize().height;
    
    // set the scaling factor to the background image
    logo->setScaleX(bX);
    logo->setScaleY(bY);
    
    logo->setScale(0.1);
    
    this->addChild(logo, 1);
    this->runAction( Sequence::create(
                                        DelayTime::create(1),
                                        CallFunc::create(CC_CALLBACK_0(Splash::dismissSplash,this)),
                                        NULL));
    //    CallFunc::create(CC_CALLBACK_1(Splash::dismissSplash, this))
    return true;
}

void Splash::dismissSplash()
{
    auto mainmenu = MainMenu::createScene();
    auto scene = TransitionFade::create(1.0f, mainmenu);
//    auto scene = TransitionPageTurn::create(1.0f, mainmenu, false);
    Director::getInstance()->replaceScene(scene);
}