//
//  GameOverScene.h
//  DragMeAway
//
//  Created by Sarah Furqan on 19/10/2014.
//
//

#ifndef __DragMeAway__GameOverScene__
#define __DragMeAway__GameOverScene__

#include "cocos2d.h"

class GameOverLayer : public cocos2d::LayerColor
{
public:
    GameOverLayer():_label(NULL) {};
    virtual ~GameOverLayer();
    bool init();
    CREATE_FUNC(GameOverLayer);
    
    void gameOverDone();


    CC_SYNTHESIZE_READONLY(cocos2d::LabelTTF*, _label, Label);
    
};

class GameOverScene : public cocos2d::Scene
{
public:
    GameOverScene():_layer(NULL) {};
    ~GameOverScene();
    bool init();
    CREATE_FUNC(GameOverScene);
    
    CC_SYNTHESIZE_READONLY(GameOverLayer*, _layer, Layer);
    
};

#endif /* defined(__DragMeAway__GameOverScene__) */
