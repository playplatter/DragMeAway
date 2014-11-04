//
//  GameOverScene.cpp
//  DragMeAway
//
//  Created by Sarah Furqan on 19/10/2014.
//
//

#include "GameOverScene.h"
#include "HelloWorldScene.h"
#include "MainMenuScene.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;

bool GameOverScene::init()
{
    if( Scene::init() )
    {
        this->_layer = GameOverLayer::create();
        this->_layer->retain();
        this->addChild(_layer);
        
        return true;
    }
    else
    {
        return false;
    }
}

GameOverScene::~GameOverScene()
{
    if (_layer)
    {
        _layer->release();
        _layer = NULL;
    }
}


bool GameOverLayer::init()
{
    if ( Layer::init() )
//        if ( LayerColor::initWithColor( Color4B(255,255,255,0) ) )
    {
        Size visibleSize = CCDirector::getInstance()->getWinSize();
        Vec2 origin = Director::getInstance()->getVisibleOrigin();
        
        
        // Title
        auto label = Label::createWithSystemFont("Game Over", "Arial", 34);
        
        
        // position the label on the center of the screen
        label->setPosition(
                           Vec2(origin.x + visibleSize.width / 2,
                                visibleSize.height - label->getContentSize().height));
        
        // add the label as a child to this layer
        this->addChild(label, 1);
        
        // Nut board image
        auto nutboard = Sprite::create("nuts_score.png");
        nutboard->setPosition(
                              Vec2(
                                   origin.x + visibleSize.width/2,
                                   origin.y + visibleSize.height  - label->getContentSize().height
                                   - nutboard->getContentSize().height));
        this->addChild(nutboard);
        
        
        
        // nuts
        this->_label = Label::createWithSystemFont("","Arial", 24);
        _label->setColor( Color3B(255, 255, 255) );
        this->addChild(_label);
        _label->setPosition(
                            Vec2(visibleSize.width/2 + nutboard->getContentSize().width/5,
                                 visibleSize.height  - label->getContentSize().height - nutboard->getContentSize().height - 5));
        
        
        
        // Title
        _score = Label::createWithSystemFont("", "Arial", 24);
        
        
        // position the label on the center of the screen
        _score->setPosition(
                           Vec2(origin.x + visibleSize.width / 2,
                                visibleSize.height/2 + _score->getContentSize().height/2 - nutboard->getContentSize().height/2));
        this->addChild(_score);
        
        
        
        // Menu items
        auto menuItem = MenuItemImage::create("btn_menu.png",
                                               "btn_menu.png",
                                               callfunc_selector(GameOverLayer::mainMenu));
        
        menuItem->setPosition(Vec2(visibleSize.width/2 - menuItem->getContentSize().width - 10, menuItem->getContentSize().height/2));
        
        auto restartItem = MenuItemImage::create("btn_restart.png",
                                               "btn_restart.png",
                                               callfunc_selector(GameOverLayer::restartGame));
        
        restartItem->setPosition(Vec2(visibleSize.width/2, restartItem->getContentSize().height/2));
        
        auto closeItem = MenuItemImage::create("btn_quit.png",
                                               "btn_quit.png",
                                               callfunc_selector(GameOverLayer::quitGame));
        
        closeItem->setPosition(Vec2(visibleSize.width/2 + closeItem->getContentSize().width +10, closeItem->getContentSize().height/2));

        
            // create menu, it's an autorelease object
            auto menu = Menu::create(restartItem, menuItem, closeItem, NULL);
            menu->setPosition(Vec2::ZERO);
            this->addChild(menu, 1);


//        this->runAction( Sequence::create(
//                                            DelayTime::create(3),
//                                            CallFunc::create(this, callfunc_selector(GameOverLayer::gameOverDone)),
//                                            NULL));
        return true;
    }
    else
    {
        return false;
    }
}

void GameOverLayer::quitGame()
{
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

void GameOverLayer::mainMenu()
{
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("explode.wav");
    auto scene = MainMenu::createScene();
    CCDirector::getInstance()->replaceScene(scene);//CCDirector::getInstance()->getRunningScene()
}

void GameOverLayer::restartGame()
{
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("explode.wav");
    auto scene = HelloWorld::createScene();
    CCDirector::getInstance()->replaceScene(scene);//CCDirector::getInstance()->getRunningScene()
}

GameOverLayer::~GameOverLayer()
{
    if (_label)
    {
//        _label->release();
        _label = NULL;
    }
}
