//
//  SelectionScene.h
//  DragMeAway
//
//  Created by Sarah Furqan on 04/11/2014.
//
//

#ifndef __DragMeAway__SelectionScene__
#define __DragMeAway__SelectionScene__

#include "cocos2d.h"
#include "PlayerSpriteLayer.h"

USING_NS_CC;

class SelectionScene : public cocos2d::Layer
{
public:
    
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    CREATE_FUNC(SelectionScene);
    
    void nextCharacter();
    void prevCharacter();
    void charactersList(Size visibleSize, Vec2 origin);
    void startGame();
    void mainMenu();
    
    PlayerSpriteLayer *_playerLayer;
    
protected:
    Vector<Sprite *> *_players;
    Vector<std::string > *_playersList;
    int currentCharacter;
    Label * _label;
    Sprite * _selectedCharacter;
    Menu * menu;
};

#endif /* defined(__DragMeAway__GameOverScene__) */
