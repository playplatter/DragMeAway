//
//  Hurdles.h
//  DragMeAway
//
//  Created by Syeda Sara Furqan on 17/11/2014.
//
//

#ifndef __DragMeAway__Hurdles__
#define __DragMeAway__Hurdles__

#include <stdio.h>
#include "cocos2d.h"
using namespace cocos2d;

class Hurdles : public Layer
{
private:
    typedef Hurdles self;
    typedef Layer super;
    
    /// The offset of a touch
    Point touchOffset;
    
    /// Returns the Cocos2d position of the touch
    Point touchToPoint(Touch* touch);
    
    /// Returns true if the touch is within the boundary of our sprite
    bool isTouchingSprite(Touch* touch);
    
    // we are multi-touch enabled, so we must use the ccTouches functions
    // vs the ccTouch functions
    //    void onTouchesBegan(const std::vector<Touch*>& touches, Event* event);
    //    void onTouchesMoved(const std::vector<Touch*>& touches, Event* event);
    //    void onTouchesEnded(const std::vector<Touch*>& touches, Event* event);
    
public:
    
    Hurdles();
    ~Hurdles();
    
    /// Our sprite
    std::string _spriteName;
    Sprite* _sprite;
    
    float _collisionRadius, _spriteScaleFactor;
    Color3B _originalTint;
    
    void createYellowBird();
    void createBlueBird();
    
    bool _isHurdle;
};

#endif /* defined(__DragMeAway__Hurdles__) */
