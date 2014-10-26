//
//  MainMenuScene.cpp
//  DragMeAway
//
//  Created by Sarah Furqan on 14/10/2014.
//
//

#include "MainMenuScene.h"
#include "HelloWorldScene.h"
#include "TouchableSpriteLayer.h"
#include "SimpleAudioEngine.h"


Scene* MainMenu::createScene() {
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = MainMenu::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool MainMenu::init() {
    //////////////////////////////
    // 1. super init first
    if (!Layer::init()) {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    
    // add static background image
    auto sky = Sprite::create("sky.png");
    
    // position the sprite on the center of the screen
    sky->setPosition(
                      Vec2(origin.x + visibleSize.width / 2,
                           origin.y + visibleSize.height / 2));
    
    this->addChild(sky, 1);
    
    
    
    /////////////////////////////
    // 2. add a play button to start the game
    
    auto playBtn = MenuItemImage::create("btn_play.png",
                                           "btn_play.png",
                                           CC_CALLBACK_1(MainMenu::menuStartCallback, this));
    
    playBtn->setPosition(
                           Vec2(
                                origin.x + visibleSize.width / 2 - playBtn->getContentSize().width,
                                origin.y));
    
    
    // add a "close" icon to exit the progress. it's an autorelease object
    auto quitBtn = MenuItemImage::create("btn_quit.png",
                                           "btn_quit.png",
                                           CC_CALLBACK_1(MainMenu::menuCloseCallback, this));
    
    quitBtn->setPosition(
                           Vec2(
                                origin.x + visibleSize.width / 2 + quitBtn->getContentSize().width,
                                origin.y));
    
    // create menu, it's an autorelease object
    auto menu = Menu::create(playBtn, quitBtn, NULL);
    menu->setPosition(Vec2(0, visibleSize.height/2));
    this->addChild(menu, 1);
    
    
    /////////////////////////////
    // 3. add your codes below...
    
    // add a label shows "Hello World"
    // create and initialize a label
    
    auto label = Label::createWithSystemFont("Drag Me Away", "Arial", 24);
    
    // position the label on the center of the screen
    label->setPosition(
                       Vec2(origin.x + visibleSize.width / 2,
                            visibleSize.height - label->getContentSize().height));
    
    // add the label as a child to this layer
    this->addChild(label, 1);
    
    auto copyright = Label::createWithSystemFont("© 2014 All Rights Reserved with Play Platter Studios", "Arial", 14);
    
    // position the label on the center of the screen
    copyright->setPosition(
                       Vec2(origin.x + visibleSize.width / 2,
                            copyright->getContentSize().height));
    
    // add the label as a child to this layer
    this->addChild(copyright, 1);
    
    return true;
}

void MainMenu::menuCloseCallback(Ref* pSender) {
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("explode.wav");
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
    MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif
    
    Director::getInstance()->end();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void MainMenu::menuStartCallback(Ref* pSender) {
    
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("explode.wav");
    
    // create a scene. it's an autorelease object
    auto scene = HelloWorld::createScene();
    
    // Start the game
    Director::getInstance()->pushScene(scene);
}