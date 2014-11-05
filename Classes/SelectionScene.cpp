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
    auto prevItem = MenuItemImage::create("btn_back.png",
                                          "btn_back.png",
                                          callfunc_selector(SelectionScene::prevCharacter));
    prevItem->setPosition(Vec2(visibleSize.width/2 - prevItem->getContentSize().width*2 - 10, prevItem->getContentSize().height/2));
    
    auto menuItem = MenuItemImage::create("btn_menu.png",
                                          "btn_menu.png",
                                          callfunc_selector(SelectionScene::mainMenu));
    menuItem->setPosition(Vec2(visibleSize.width/2 - menuItem->getContentSize().width/2 - 10, menuItem->getContentSize().height/2));
    
    auto playItem = MenuItemImage::create("btn_play.png",
                                           "btn_play.png",
                                           callfunc_selector(SelectionScene::startGame));
    playItem->setPosition(Vec2(visibleSize.width/2 + playItem->getContentSize().width/2, playItem->getContentSize().height/2));
    
    auto nextItem = MenuItemImage::create("btn_next.png",
                                           "btn_next.png",
                                           callfunc_selector(SelectionScene::nextCharacter));
    nextItem->setPosition(Vec2(visibleSize.width/2 + nextItem->getContentSize().width*2 +10, nextItem->getContentSize().height/2));

    
    // create menu, it's an autorelease object
    auto menu = Menu::create(prevItem, playItem, menuItem, nextItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
    
    
    
    this->characterList();
    
    printf("%2lu", _players->size());
    
    return true;
}

void SelectionScene::characterList(){
    
    
    Size visibleSize = CCDirector::getInstance()->getWinSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
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
    character2->setScale(0.1);
    character2->setPosition(
                            Vec2(
                                 origin.x + visibleSize.width/2,
                                 origin.y + visibleSize.height/2
                                 //                                   - label->getContentSize().height
                                 //                                   - character->getContentSize().height
                                 ));
    this->addChild(character2);
    
    
    ////////////////////////////////////////////////////////////
    this->_players = new Vector<Sprite *>(2);
    this->_players->insert(0, character);
    this->_players->insert(1, character2);
    this->_players->shrinkToFit();
    currentCharacter = 1;
}

void SelectionScene::startGame()
{
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("explode.wav");
    auto scene = HelloWorld::createScene();
    CCDirector::getInstance()->replaceScene(scene);
}

void SelectionScene::mainMenu()
{
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("explode.wav");
    auto scene = MainMenu::createScene();
    CCDirector::getInstance()->replaceScene(scene);//CCDirector::getInstance()->getRunningScene()
}

void SelectionScene::prevCharacter()
{
    this->characterList();
    printf("%2d / %2lu", currentCharacter, this->_players->size());
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("explode.wav");
    
    for (int j = 0; j < this->_players->size(); j++) {
        // set visiblity false
        Sprite * player = this->_players->at(j);
        player->setVisible(false);
    }
//    for (auto player: *this->_players){
//        // set visiblity false
//       ((Sprite *) player)->setVisible(false);
//    }
    
    if (currentCharacter > 0) {
        currentCharacter -- ;
    } else
        currentCharacter = this->_players->size() - 1;
    
    ((Sprite *) this->_players->at(currentCharacter))->setVisible(false);
}

void SelectionScene::nextCharacter()
{
    this->characterList();
    printf("%2d / %2lu", currentCharacter, this->_players->size());
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("explode.wav");
    
    for (int j = 0; j < this->_players->size(); j++) {
        // set visiblity false
        Sprite * player = this->_players->at(j);
        player->setVisible(false);
    }
    
//    for (auto player: *this->_players){
//        ((Sprite *) player)->setVisible(false);
//    }
    
    if (currentCharacter < this->_players->size()-1) {
        currentCharacter ++ ;
    } else
        currentCharacter = 0;
    ((Sprite *) this->_players->at(currentCharacter))->setVisible(false);
}
