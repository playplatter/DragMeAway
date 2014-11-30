//
//  PlayerSpriteLayer.h
//  DragMeAway
//
//  Created by Sarah Furqan on 14/10/2014.
//
//

#ifndef __DragMeAway__PlayerSpriteLayer__
#define __DragMeAway__PlayerSpriteLayer__

#include <stdio.h>
#include "cocos2d.h"
using namespace cocos2d;

class PlayerSpriteLayer : public Layer
{
private:
    typedef PlayerSpriteLayer self;
    typedef Layer super;
    
    /// The offset of a touch
    Point touchOffset;
    
    
    // Singleton
//    static PlayerSpriteLayer *s_instance;
    
public:
//    static PlayerSpriteLayer *getInstance(void);
//    static void destroyInstance(void);
    
    PlayerSpriteLayer();
    ~PlayerSpriteLayer();
    
    // MUST BE CALLED!
//    void setActor(std::string spriteName, Sprite *sprite);
//    void addActorLayer();

    
    /// Our sprite
    std::string _spriteName;
    Sprite* _sprite;
    
    float _collisionRadius, _spriteScaleFactor;
    Color3B _originalTint;
    
    /// Returns the Cocos2d position of the touch
    Point touchToPoint(Touch* touch);
    
    /// Returns true if the touch is within the boundary of our sprite
    bool isTouchingSprite(Touch* touch);
    
    // we are multi-touch enabled, so we must use the ccTouches functions
    // vs the ccTouch functions
    void onTouchesBegan(const std::vector<Touch*>& touches, Event* event);
    void onTouchesMoved(const std::vector<Touch*>& touches, Event* event);
    void onTouchesEnded(const std::vector<Touch*>& touches, Event* event);
};

#endif /* defined(__DragMeAway__PlayerSpriteLayer__) */
