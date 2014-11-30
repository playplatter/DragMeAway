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

Scene* SelectionScene::createScene() {
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = SelectionScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool SelectionScene::init() {
    
    //////////////////////////////
    // 1. super init first
    if (!Layer::init()) {
        return false;
    }
    
    Size visibleSize = CCDirector::getInstance()->getWinSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    
    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("lemoncreme.wav", true);
    
    
    
    
    // Title
    auto label = Label::createWithSystemFont("Character Selection", "Noteworthy", 34);
    
    label->setCascadeColorEnabled(true);
    label->setColor(Color3B(240,100,0));
    
    // position the label on the center of the screen
    label->setPosition(
                       Vec2(origin.x + visibleSize.width / 2,
                            visibleSize.height - label->getContentSize().height/2));
    
    // add the label as a child to this layer
    this->addChild(label, 1);
    
    
    
    
    // Menu items
    auto menuItem = MenuItemImage::create("btn_menu.png",
                                          "btn_menu.png",
                                          CC_CALLBACK_0(SelectionScene::mainMenu, this));
    menuItem->setPosition(Vec2(menuItem->getContentSize().width/2 - 10, menuItem->getContentSize().height/2));
    menuItem->setScale(0.5);
    
    auto playItem = MenuItemImage::create("btn_play.png",
                                           "btn_play.png",
                                           CC_CALLBACK_0(SelectionScene::startGame, this));
    playItem->setPosition(Vec2(visibleSize.width - playItem->getContentSize().width/2, playItem->getContentSize().height/2));
    playItem->setScale(0.5);
    
    auto prevItem = MenuItemImage::create("btn_back.png",
                                          "btn_back.png",
                                          CC_CALLBACK_0(SelectionScene::prevCharacter, this));
    prevItem->setPosition(Vec2(visibleSize.width/2 - prevItem->getContentSize().width,
//                               prevItem->getContentSize().height/2
                               visibleSize.height/2));
    prevItem->setScale(0.5);
    
    auto nextItem = MenuItemImage::create("btn_next.png",
                                           "btn_next.png",
                                           CC_CALLBACK_0(SelectionScene::nextCharacter, this));
    nextItem->setPosition(Vec2(visibleSize.width/2 + nextItem->getContentSize().width,
//                               nextItem->getContentSize().height/2
                               visibleSize.height/2));
    nextItem->setScale(0.5);

    
    // create menu, it's an autorelease object
    menu = Menu::create(prevItem, playItem, menuItem, nextItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
    
    
    
    ////////////////////////////////////////// PLAYER CONFIGURATION ////////////////////////////////////////////////
    
    this->charactersList( visibleSize, origin );
    return true;
}

void SelectionScene::charactersList(Size visibleSize, Vec2 origin){
    
    currentCharacter = 0;
    
    ///////////////////////////////////////////////////////////
    // character image
    
    auto character = Sprite::create("Kuchu_00000.png");
    character->setPosition(
                           Vec2(
                                origin.x + visibleSize.width/2,
                                origin.y + visibleSize.height/2
                                //                                   - label->getContentSize().height
                                //                                   - character->getContentSize().height
                                ));
    character->setVisible(false);
//    character->setScale(0.4);
    character->setName("Kuchu_");
    
    this->addChild(character);
    character->autorelease();
    
    auto character2 = Sprite::create("Muchu_00000.png");
    character2->setPosition(
                            Vec2(
                                 origin.x + visibleSize.width/2,
                                 origin.y + visibleSize.height/2
                                 //                                   - label->getContentSize().height
                                 //                                   - character->getContentSize().height
                                 ));
    character2->setVisible(false);
//    character2->setScale(0.4);
    character2->setName("Muchu_");
    this->addChild(character2);
    character2->autorelease();
    
    ////////////////////////////////////////////////////////////
    _players = new Vector<Sprite *>();
    _players->pushBack(character);
    _players->pushBack(character2);
    
//    printf("%2lu", _players->size());
    
    // set actor layer
    _selectedCharacter = (Sprite *) _players->at(currentCharacter);
    _selectedCharacter->setVisible(true);
    UserDefault::getInstance()->setStringForKey("spriteName", _selectedCharacter->getName().c_str());
    UserDefault::getInstance()->setFloatForKey("scaleFactor", _selectedCharacter->getScale());
    
}

void SelectionScene::startGame()
{
    //hide buttons
    menu->setVisible(false);
    CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
//    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("explode.wav");
//    printf("\n spritename %s",  _selectedCharacter->getName().c_str());
//    //remove everything
//    for (int i = 0; i <_players->size(); i++) {
//        this->removeChild(_players->at(i));
//    }
//    _players = NULL;
    
    auto scene = HelloWorld::createScene();
    //_playerLayer;
    Director::getInstance()->replaceScene(scene);
}

void SelectionScene::mainMenu()
{
    /// hide buttons
    menu->setVisible(false);
    CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("explode.wav");
    auto scene = MainMenu::createScene();
    Director::getInstance()->replaceScene(scene); // Director::getInstance()->getRunningScene()
}

void SelectionScene::prevCharacter()
{
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("explode.wav");
    for (int j = 0; j < _players->size(); j++) {
        // set visiblity false
        Sprite * player = (Sprite*)_players->at(j);
        player->setVisible(false);
    }
    if (currentCharacter > 0) {
        currentCharacter -- ;
    } else
        currentCharacter = _players->size() - 1;
    
    // set actor layer
    _selectedCharacter = (Sprite *) _players->at(currentCharacter);
    _selectedCharacter->setVisible(true);
    UserDefault::getInstance()->setStringForKey("spriteName", _selectedCharacter->getName().c_str());
    UserDefault::getInstance()->setFloatForKey("scaleFactor", _selectedCharacter->getScale());
    
//    printf("\n%s %.1f", _selectedCharacter->getName().c_str(), _selectedCharacter->getScale());
}

void SelectionScene::nextCharacter()
{
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("explode.wav");
    for (int j = 0; j < _players->size(); j++) {
        // set visiblity false
        Sprite * player = (Sprite*) _players->at(j);
        player->setVisible(false);
    }
    if (currentCharacter < _players->size()-1) {
        currentCharacter ++ ;
    } else
        currentCharacter = 0;
    
    // set actor layer
    _selectedCharacter = (Sprite *) _players->at(currentCharacter);
    _selectedCharacter->setVisible(true);
    UserDefault::getInstance()->setStringForKey("spriteName", _selectedCharacter->getName().c_str());
    UserDefault::getInstance()->setFloatForKey("scaleFactor", _selectedCharacter->getScale());
    
//    printf("\n%s", _selectedCharacter->getName().c_str());
}
