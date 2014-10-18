#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "ParallaxNodeExtras.h"

USING_NS_CC;

class HelloWorld : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);

private:
    // update position of scrolling background
    // scheduled Update
    void update(float dt);
    void createScrollingBackground(cocos2d::Size visibleSize, cocos2d::Vec2 origin);

    //Create the CCParallaxNode
    ParallaxNodeExtras * _backgroundNode;
    
    Sprite *_background1, *_background2, *_background3, *_background4, *_grass, *_grass2, *_tree;
    Vector<Sprite*> _collectables;
    Vector<Sprite*> _hurdles;
};

#endif // __HELLOWORLD_SCENE_H__
