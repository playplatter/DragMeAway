#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "ParallaxNodeExtras.h"
#include "PlayerSpriteLayer.h"

USING_NS_CC;

class HelloWorld : public cocos2d::Layer
{
    
public:
    
    PlayerSpriteLayer* actor;

    
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // a selector callback
    void mainMenu(cocos2d::Ref* pSender);
    void pauseGame(cocos2d::Ref* pSender);
    void resumeGame(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
    
    float randomValueBetween(float low, float high);
    void setInvisible(Node * node);
    float getTimeTick();
    
    // RenderTexture
    RenderTexture *_rt;
    
    /// The offset of a touch
    Point touchOffset;
    
    /// Returns the Cocos2d position of the touch
    Point touchToPoint(Touch* touch);
    
    /// Returns true if the touch is within the boundary of our sprite
    bool isTouchingSprite(Touch* touch);
    
    // Enable Touch
//    void onTouchesBegan(const std::vector<Touch*>& touches, Event* event);
//    void onTouchesMoved(const std::vector<Touch*>& touches, Event* event);
//    void onTouchesEnded(const std::vector<Touch*>& touches, Event* event);
    
    bool isHintShown;
    
    void gameOver();
    void stopEverythingMoving();
    void removeSheildEffect();
    
    LayerColor *pausescreenbarbot, *pausescreenbartop;
    
    float _playerCollisionRadius, _hurdleCollisionRadius, _nutsCollisionRadius;
    
protected:
    Vector<Sprite *> *_nuts, *_collectables, *_hurdles;
    Animate *_animateBird;
private:
    
    typedef HelloWorld self;
    typedef Layer super;
    
    PhysicsWorld * _sceneWorld;
    void setPhysicsWorld(PhysicsWorld *world){_sceneWorld = world;}
    bool onContactBegin(PhysicsContact &contact);
    
    void startSpawning();
    
    // update position of scrolling background
    // scheduled Update
    void update(float dt);
    
    void createScrollingBackground(cocos2d::Size visibleSize, cocos2d::Vec2 origin);
    void createTopbarStats(cocos2d::Size visibleSize, cocos2d::Vec2 origin);
    void createScrollingTargets(cocos2d::Size visibleSize, cocos2d::Vec2 origin);
    
    void showHint();
    void hideHint();
    void updateBackground(float dt);
    void updateNuts(float dt);
    void updateCollectables(float dt);
    void updateTargets(float dt);
    
    void makeActorVisible();
    
    EventListenerTouchAllAtOnce *_listener;
    void onTouchesBegan(const std::vector<Touch*>& touches, Event* event);
    
    Size visibleSize;
    Vec2 origin;
    
    //Create the CCParallaxNode
    ParallaxNodeExtras * _backgroundNode, * _targetNode;
    
    // Create World Sprites
    Sprite *_background1, *_background2, *_background3, *_background4,
    *_street1, *_street2, *_street3, *_street4, *_street5,
    *_grass, *_grass2, *_tree, *_hintSprite;
    
    // Score Label
    Label *_llives, *_lscore, *_lnuts, *_lcoins, *_label;
    
    Color3B _oldtint;
    
    int _score;
    int _lives, _nutscore, _coins;
    bool isSheilded, isSheildVisible, isPaused = false, shouldSpawn;
    
    int _nextCollectable, _nextNut, _nextHurdle;
    float _nextCollectableSpawn, _nextNutSpawn, _nextHurdleSpawn;
    
    MenuItemImage *_backItem, *_playItem, *_pauseItem;
};

#endif // __HELLOWORLD_SCENE_H__
