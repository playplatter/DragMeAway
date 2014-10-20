//
//  TouchableSpriteLayer.h
//  DragMeAway
//
//  Created by Sarah Furqan on 14/10/2014.
//
//

#ifndef __DragMeAway__TouchableSpriteLayer__
#define __DragMeAway__TouchableSpriteLayer__

#include <stdio.h>
#include "cocos2d.h"
using namespace cocos2d;

class TouchableSpriteLayer : public Layer
{
private:
    typedef TouchableSpriteLayer self;
    typedef Layer super;
    
    /// The offset of a touch
    Point touchOffset;
    
public:
    TouchableSpriteLayer();
    ~TouchableSpriteLayer();
    
    /// Our sprite
    Sprite* _sprite;
    
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

#endif /* defined(__DragMeAway__TouchableSpriteLayer__) */
