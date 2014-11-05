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
    void characterList();
    void startGame();
    void mainMenu();
    
private:
    Vector<Sprite *> *_players;
    int currentCharacter;
    Label * _label;
    
};

#endif /* defined(__DragMeAway__GameOverScene__) */
