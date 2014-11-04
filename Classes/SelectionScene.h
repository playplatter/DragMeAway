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

class SelectionLayer : public cocos2d::Layer //LayerColor
{
public:
	SelectionLayer():_label(NULL) {};
    virtual ~SelectionLayer();
    bool init();
    CREATE_FUNC(SelectionLayer);
    
    void nextCharacter();
    void prevCharacter();
    void startGame();
    void mainMenu();
    
    Vector<Sprite *> characters;
    cocos2d::Label * _score;
    
    CC_SYNTHESIZE_READONLY(cocos2d::Label*, _label, Label);
    
};

class SelectionScene : public cocos2d::Scene
{
public:
	SelectionScene():_layer(NULL) {};
    ~SelectionScene();
    bool init();
    CREATE_FUNC(SelectionScene);
    
    CC_SYNTHESIZE_READONLY(SelectionLayer*, _layer, Layer);
    
};

#endif /* defined(__DragMeAway__GameOverScene__) */
