//
//  MainMenuScene.h
//  DragMeAway
//
//  Created by Sarah Furqan on 14/10/2014.
//
//

#ifndef __DragMeAway__MainMenuScene__
#define __DragMeAway__MainMenuScene__

#include "cocos2d.h"

USING_NS_CC;

class MainMenu : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(MainMenu);
    
    // selector callbacks
    void menuCloseCallback(cocos2d::Ref* pSender);
    void menuStartCallback(cocos2d::Ref* pSender);
    
};

#endif /* defined(__DragMeAway__MainMenuScene__) */
