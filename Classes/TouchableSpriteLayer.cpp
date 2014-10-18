//
//  TouchableSprite.cpp
//  DragMeAway
//
//  Created by Sarah Furqan on 14/10/2014.
//
//

#include "TouchableSpriteLayer.h"
#include "SimpleAudioEngine.h"

TouchableSpriteLayer::TouchableSpriteLayer()
{
    Size iSize = Director::getInstance()->getWinSize();
    
    // create sprite
    _sprite = Sprite::create("birdAmin0000.png");
    this->_sprite->setPosition(Point(iSize.width / 2.0f, iSize.height / 2.0f));
    this->_sprite->setAnchorPoint(Point(0.5f, 0.55f)); // nudge the anchor point upward because of the shadow
    this->addChild(this->_sprite, 2);
    
    Vector<SpriteFrame*> animFrames(15);
    char str[100] = {0};
    for(int i = 1; i < 11; i++)
    {
        sprintf(str, "birdAmin00%02d.png",i);
        auto frame = SpriteFrame::create(str,Rect(0,0,60,70)); //we assume that the sprites' dimentions are 40*40 rectangles.
        animFrames.pushBack(frame);
    }
    
    auto animation = Animation::createWithSpriteFrames(animFrames, 0.2f);
    animation->setLoops(-1);
    auto animate = Animate::create(animation);
    _sprite->runAction(animate);
    
    // listen for touch events
    auto listener = EventListenerTouchAllAtOnce::create();
    listener->onTouchesBegan = CC_CALLBACK_2(self::onTouchesBegan, this);
    listener->onTouchesMoved = CC_CALLBACK_2(self::onTouchesMoved, this);
    listener->onTouchesEnded = CC_CALLBACK_2(self::onTouchesEnded, this);
    listener->onTouchesCancelled = CC_CALLBACK_2(self::onTouchesEnded, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}

TouchableSpriteLayer::~TouchableSpriteLayer()
{
    // release our sprite and layer so that it gets dealloced
    CC_SAFE_RELEASE_NULL(this->_sprite);
    CC_SAFE_RELEASE_NULL(this->colorLayer);
}

Point TouchableSpriteLayer::touchToPoint(Touch* touch)
{
    // convert the touch object to a position in our cocos2d space
    return CCDirector::getInstance()->convertToGL(touch->getLocationInView());
}

bool TouchableSpriteLayer::isTouchingSprite(Touch* touch)
{
    // here's one way, but includes the rectangular white space around our sprite
    //return CGRectContainsPoint(this->sprite->boundingBox(), this->touchToPoint(touch));
    
    // this way is more intuitive for the user because it ignores the white space.
    // it works by calculating the distance between the sprite's center and the touch point,
    // and seeing if that distance is less than the sprite's radius
    return (this->_sprite->getPosition().getDistance(this->touchToPoint(touch)) < 100.0f);
}

void TouchableSpriteLayer::onTouchesBegan(const std::vector<Touch*>& touches, Event* event)
{
    // reset touch offset
    this->touchOffset = Point::ZERO;
    
    for( auto touch : touches )
    {
        // if this touch is within our sprite's boundary
        if( touch && this->isTouchingSprite(touch) )
        {
            // calculate offset from sprite to touch point
            this->touchOffset = this->_sprite->getPosition() - this->touchToPoint(touch);
            
            this->_sprite->setScale(1.0f);
            
            // animate letting go of the sprite
            this->_sprite->runAction(Sequence::create(
                                                      ScaleBy::create(0.125f, 1.111f),
                                                      ScaleBy::create(0.125f, 0.9f),
                                                      nullptr
                                                      ));
        }
    }
}

void TouchableSpriteLayer::onTouchesMoved(const std::vector<Touch*>& touches, Event* event)
{
    for( auto touch : touches )
    {
        // set the new sprite position
        if( touch && touchOffset.x && touchOffset.y )
            this->_sprite->setPosition(this->touchToPoint(touch) + this->touchOffset);
    }
}

void TouchableSpriteLayer::onTouchesEnded(const std::vector<Touch*>& touches, Event* event)
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    for( auto touch : touches )
    {
        if( touch && touchOffset.x && touchOffset.y  )
        {
            CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();

            // set the new sprite position
            this->_sprite->setPosition(this->touchToPoint(touch) + this->touchOffset);
            
            // stop any existing actions and reset the scale
            this->_sprite->stopAllActions();
            
            // animate falling of the sprite
            this->_sprite->runAction(Sequence::create(
                                                      RotateBy::create(0.1, 90),
                                                      MoveBy::create(0.8, Vec2(0, -visibleSize.height)),
                                                      nullptr
                                                      ));

            
            //	And play the sound effect in ccTouchesEnded() when the bullet is fired.
            
            // cpp with cocos2d-x
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(
            "girlscream.wav");
        }
    }
}