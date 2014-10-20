#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "ParallaxNodeExtras.h"
#include "TouchableSpriteLayer.h"

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
    
    TouchableSpriteLayer * actor;
    
    float randomValueBetween(float low, float high);
    void setInvisible(Node * node);
    float getTimeTick();
    
    
    /// The offset of a touch
    Point touchOffset;
    
    /// Returns the Cocos2d position of the touch
    Point touchToPoint(Touch* touch);
    
    /// Returns true if the touch is within the boundary of our sprite
    bool isTouchingSprite(Touch* touch);
    
    // Enable Touch
    void onTouchesBegan(const std::vector<Touch*>& touches, Event* event);
    void onTouchesMoved(const std::vector<Touch*>& touches, Event* event);
    void onTouchesEnded(const std::vector<Touch*>& touches, Event* event);
    
protected:
    Vector<Sprite *> *_collectables;
    Vector<Sprite *> *_hurdles;
    
private:
    
    typedef HelloWorld self;
    typedef Layer super;
    
    // update position of scrolling background
    // scheduled Update
    void update(float dt);
    
    void createScrollingBackground(cocos2d::Size visibleSize, cocos2d::Vec2 origin);
    
    void createScrollingTargets(cocos2d::Size visibleSize, cocos2d::Vec2 origin);
    
    void updateBackground(float dt);
    void updateTargets(float dt);
    
    Size visibleSize;
    Vec2 origin;
    
    //Create the CCParallaxNode
    ParallaxNodeExtras * _backgroundNode, * _targetNode;
    
    // Create World Sprites
    Sprite *_background1, *_background2, *_background3, *_background4, *_grass, *_grass2, *_tree;
    
    // Score Label
    Label *_label;
    
    int _score, _lives, _nextCollectable;
    float _nextCollectableSpawn;
};

#endif // __HELLOWORLD_SCENE_H__
