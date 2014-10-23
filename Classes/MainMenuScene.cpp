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
    
    /////////////////////////////
    // 2. add a menu item with "Play" or "i>" image, which is clicked to start the game
    
    auto startBtn = MenuItemImage::create("StartBtn.png",
                                           "StartBtn.png",
                                           CC_CALLBACK_1(MainMenu::menuStartCallback, this));
    
    startBtn->setPosition(
                           Vec2(
                                origin.x + visibleSize.width / 2,
                                origin.y + visibleSize.height / 2));
    
    startBtn->setScale(0.2);
    
    
    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create("CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(MainMenu::menuCloseCallback, this));
    
    closeItem->setPosition(
                           Vec2(
                                origin.x + visibleSize.width
                                - closeItem->getContentSize().width / 2,
                                origin.y + closeItem->getContentSize().height / 2));
    
    // create menu, it's an autorelease object
    auto menu = Menu::create(startBtn, closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
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
    
    return true;
}

void MainMenu::menuCloseCallback(Ref* pSender) {
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
    
    // create a scene. it's an autorelease object
    auto scene = HelloWorld::createScene();
    
    // Start the game
    Director::getInstance()->pushScene(scene);
}