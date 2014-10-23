//
//  SplashScreem.h
//  DragMeAway
//
//  Created by Syeda Sara Furqan on 23/10/2014.
//
//

#ifndef __DragMeAway__SplashScreen__
#define __DragMeAway__SplashScreen__

#include "cocos2d.h"

USING_NS_CC;

class Splash : public cocos2d::LayerColor
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // dismiss the splash screen after sometime.
    void dismissSplash();
    
    // implement the "static create()" method manually
    CREATE_FUNC(Splash);
};

#endif /* defined(__DragMeAway__SplashScreen__) */
