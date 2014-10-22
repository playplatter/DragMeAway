//
//  GameOverScene.cpp
//  DragMeAway
//
//  Created by Sarah Furqan on 19/10/2014.
//
//

#include "GameOverScene.h"
#include "HelloWorldScene.h"

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
    if ( LayerColor::initWithColor( Color4B(255,255,255,255) ) )
    {
        Size winSize = CCDirector::getInstance()->getWinSize();
        Vec2 origin = Director::getInstance()->getVisibleOrigin();
        
        this->_label = CCLabelTTF::create("","Arial", 32);
        _label->retain();
        _label->setColor( Color3B(0, 0, 0) );
        _label->setPosition(Vec2(winSize.width/2, winSize.height/2));
        this->addChild(_label);
        

        auto closeItem = MenuItemImage::create("CloseNormal.png",
                                                   "CloseSelected.png",
                                                   callfunc_selector(GameOverLayer::gameOverDone));

            closeItem->setPosition(Vec2(winSize.width/2, winSize.height/3.5));

        
            // create menu, it's an autorelease object
            auto menu = Menu::create(closeItem, NULL);
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

void GameOverLayer::gameOverDone()
{
    auto scene = HelloWorld::createScene();
    CCDirector::getInstance()->replaceScene(scene);//CCDirector::getInstance()->getRunningScene()
}

GameOverLayer::~GameOverLayer()
{
    if (_label)
    {
        _label->release();
        _label = NULL;
    }
}
