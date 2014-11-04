//
//  SelectionScene.cpp
//  DragMeAway
//
//  Created by Sarah Furqan on 19/10/2014.
//
//

#include "SelectionScene.h"
#include "HelloWorldScene.h"
#include "MainMenuScene.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;

bool SelectionScene::init()
{
    if( Scene::init() )
    {
        this->_layer = SelectionLayer::create();
        this->_layer->retain();
        this->addChild(_layer);
        
        return true;
    }
    else
    {
        return false;
    }
}

SelectionScene::~SelectionScene()
{
    if (_layer)
    {
        _layer->release();
        _layer = NULL;
    }
}


bool SelectionLayer::init()
{
    if ( Layer::init() )
//        if ( LayerColor::initWithColor( Color4B(255,255,255,0) ) )
    {
        Size visibleSize = CCDirector::getInstance()->getWinSize();
        Vec2 origin = Director::getInstance()->getVisibleOrigin();
        
        
        // Title
        auto label = Label::createWithSystemFont("Character Selection", "Noteworthy", 34);
        
        label->setCascadeColorEnabled(true);
        label->setColor(Color3B(240,100,0));
        
        // position the label on the center of the screen
        label->setPosition(
                           Vec2(origin.x + visibleSize.width / 2,
                                visibleSize.height - label->getContentSize().height));
        
        // add the label as a child to this layer
        this->addChild(label, 1);
        
        
        
        
        
        ///////////////////////////////////////////////////////////
        // character image
        
        auto character = Sprite::create("birdAmin0000.png");
        character->setPosition(
                               Vec2(
                                    origin.x + visibleSize.width/2,
                                    origin.y + visibleSize.height/2
                                    //                                   - label->getContentSize().height
                                    //                                   - character->getContentSize().height
                                    ));
        character->setVisible(false);
        this->addChild(character);
        auto character2 = Sprite::create("Kuchu0000.png");
        character2->setPosition(
                               Vec2(
                                    origin.x + visibleSize.width/2,
                                    origin.y + visibleSize.height/2
                                    //                                   - label->getContentSize().height
                                    //                                   - character->getContentSize().height
                                    ));
        this->addChild(character2);
        
        ////////////////////////////////////////////////////////////
        
        
        
        
        
        
        // Menu items
        auto prevItem = MenuItemImage::create("btn_back.png",
                                              "btn_back.png",
                                              callfunc_selector(SelectionLayer::prevCharacter));
        prevItem->setPosition(Vec2(visibleSize.width/2 - prevItem->getContentSize().width - 10, prevItem->getContentSize().height/2));
        
        auto menuItem = MenuItemImage::create("btn_menu.png",
                                              "btn_menu.png",
                                              callfunc_selector(SelectionLayer::mainMenu));
        menuItem->setPosition(Vec2(visibleSize.width/2 - menuItem->getContentSize().width - 10, menuItem->getContentSize().height/2));
        
        auto playItem = MenuItemImage::create("btn_play.png",
                                               "btn_play.png",
                                               callfunc_selector(SelectionLayer::startGame));
        playItem->setPosition(Vec2(visibleSize.width/2 + playItem->getContentSize().width, playItem->getContentSize().height/2));
        
        auto nextItem = MenuItemImage::create("btn_next.png",
                                               "btn_next.png",
                                               callfunc_selector(SelectionLayer::nextCharacter));
        nextItem->setPosition(Vec2(visibleSize.width/2 + nextItem->getContentSize().width*2 +10, nextItem->getContentSize().height/2));

        
        // create menu, it's an autorelease object
        auto menu = Menu::create(playItem, menuItem, nextItem, NULL);
        menu->setPosition(Vec2::ZERO);
        this->addChild(menu, 1);
        return true;
    }
    else
    {
        return false;
    }
}

void SelectionLayer::startGame()
{
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("explode.wav");
    auto scene = HelloWorld::createScene();
    CCDirector::getInstance()->replaceScene(scene);
}

void SelectionLayer::mainMenu()
{
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("explode.wav");
    auto scene = MainMenu::createScene();
    CCDirector::getInstance()->replaceScene(scene);//CCDirector::getInstance()->getRunningScene()
}

void SelectionLayer::prevCharacter()
{
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("explode.wav");
}

void SelectionLayer::nextCharacter()
{
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("explode.wav");
}





SelectionLayer::~SelectionLayer()
{
    if (_label)
    {
//        _label->release();
        _label = NULL;
    }
}
