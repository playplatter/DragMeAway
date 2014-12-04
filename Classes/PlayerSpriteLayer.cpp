//
//  TouchableSprite.cpp
//  DragMeAway
//
//  Created by Sarah Furqan on 14/10/2014.
//
//

#include "PlayerSpriteLayer.h"
#include "SimpleAudioEngine.h"
#include "HelloWorldScene.h"

PlayerSpriteLayer::~PlayerSpriteLayer()
{
    // release our sprite and layer so that it gets dealloced
//    CC_SAFE_RELEASE_NULL(this->_sprite);
    
}

PlayerSpriteLayer::PlayerSpriteLayer()
{
    Size visibleSize = Director::getInstance()->getWinSize();
    
    _spriteName = UserDefault::getInstance()->getStringForKey("spriteName", "Kuchu_");// Config::_spriteName;
    _spriteScaleFactor = UserDefault::getInstance()->getFloatForKey("scaleFactor", 1.0);
    
    
    // create sprite
    _sprite = Sprite::create(_spriteName+"00000.png"); //    _sprite = Sprite::create("Kuchu0000.png"); || _sprite = Config::_sprite;
    
    
    
    _originalTint = this->_sprite->getColor();
    _collisionRadius = this->_sprite->getBoundingBox().size.width*0.2;
    
    if(this->getChildByName(_spriteName) == nullptr){
        this->addChild(_sprite, 2);
        _sprite->autorelease(); // addChild() retained so we release
    }
    
    Vector<SpriteFrame*> animFrames(11);
    char str[100] = {0};
    for(int i = 1; i < 11; i++)
    {
        sprintf(str, "%05d.png",i);
        printf("\n%s", (_spriteName+str).c_str());
        auto frame = SpriteFrame::create((_spriteName+str).c_str(),
                                         Rect(0,0,_sprite->getBoundingBox().size.width,
                                              _sprite->getBoundingBox().size.height)); // we assume that the sprites' dimentions are 40*40 rectangles.
        animFrames.pushBack(frame);
    }
    
    auto animation = Animation::createWithSpriteFrames(animFrames, 0.2f);
    animation->setLoops(-1);
    animation->setDelayPerUnit(0.05);
    auto animate = Animate::create(animation);

    this->_sprite->setScale(0.6);//_spriteScaleFactor
    _sprite->setAnchorPoint(Point(0,0)); // nudge the anchor point upward because of the shadow
    
    
    
    _sprite->setPosition(Point(visibleSize.width / 2.0f, visibleSize.height - _sprite->getBoundingBox().size.height));
    
    printf("\n\nscale = %f\n\n",_spriteScaleFactor);
    _sprite->runAction(animate);
    
    auto spritebody = PhysicsBody::createCircle(_collisionRadius);
    spritebody->setCollisionBitmask(1);
    spritebody->setContactTestBitmask(true);
    _sprite->setPhysicsBody(spritebody);
    
    // listen for touch events
    auto listener = EventListenerTouchAllAtOnce::create();
    listener->onTouchesBegan = CC_CALLBACK_2(self::onTouchesBegan, this);
    listener->onTouchesMoved = CC_CALLBACK_2(self::onTouchesMoved, this);
    listener->onTouchesEnded = CC_CALLBACK_2(self::onTouchesEnded, this);
    listener->onTouchesCancelled = CC_CALLBACK_2(self::onTouchesEnded, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
}

Point PlayerSpriteLayer::touchToPoint(Touch* touch)
{
    // convert the touch object to a position in our cocos2d space
    return CCDirector::getInstance()->convertToGL(touch->getLocationInView());
}

bool PlayerSpriteLayer::isTouchingSprite(Touch* touch)
{
    // here's one way, but includes the rectangular white space around our sprite
    //return CGRectContainsPoint(this->sprite->boundingBox(), this->touchToPoint(touch));
    
    // this way is more intuitive for the user because it ignores the white space.
    // it works by calculating the distance between the sprite's center and the touch point,
    // and seeing if that distance is less than the sprite's radius
    return (this->_sprite->getPosition().getDistance(this->touchToPoint(touch)) < 50.0f);
}

void PlayerSpriteLayer::onTouchesBegan(const std::vector<Touch*>& touches, Event* event)
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    auto w = this->_sprite->getBoundingBox().size.width/2;
    auto h = this->_sprite->getBoundingBox().size.height/2;
    
    // reset touch offset
    this->touchOffset = Point::ZERO;
    
    for( auto touch : touches )
    {
        auto xpos = this->touchToPoint(touch).x + this->touchOffset.x;
        auto ypos =  this->touchToPoint(touch).y + this->touchOffset.y;
        
        // if this touch is within our sprite's boundary
        if( touch && (xpos > w && ypos > h && xpos < visibleSize.width - w && ypos < visibleSize.height - h) )// && this->isTouchingSprite(touch) )
        {
//        	CocosDenshion::SimpleAudioEngine::getInstance()->stopAllEffects();
            // calculate offset from sprite to touch point
            this->touchOffset = this->_sprite->getPosition() - this->touchToPoint(touch);
//            auto distance = this->touchToPoint(touch).getDistance(this->_sprite->getPosition());
//            float speed = 2.0f;
//            auto time  = distance / speed;
            
            // move Sprite to touched location
//            this->_sprite->runAction(MoveTo::create(2.0, Vec2(touchToPoint(touch).x, touchToPoint(touch).y)));
            
//            this->_sprite->setScale(1.0f);
//            
//            // animate letting go of the sprite
//            this->_sprite->runAction(Sequence::create(
//                                                      ScaleBy::create(0.125f, 1.111f),
//                                                      ScaleBy::create(0.125f, 0.9f),
//                                                      nullptr
//                                                      ));
        }
    }
}

void PlayerSpriteLayer::onTouchesMoved(const std::vector<Touch*>& touches, Event* event)
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    auto w = this->_sprite->getBoundingBox().size.width/2;
    auto h = this->_sprite->getBoundingBox().size.height/2;
    
    for( auto touch : touches )
    {
        auto xpos = this->touchToPoint(touch).x + this->touchOffset.x;
        auto ypos =  this->touchToPoint(touch).y + this->touchOffset.y;
        
        // set the new sprite position
        if( touch && touchOffset.x && touchOffset.y
           && (xpos > w && ypos > h && xpos < visibleSize.width - w && ypos < visibleSize.height - h) )
        {
            this->_sprite->setPosition(this->touchToPoint(touch) + this->touchOffset);
        }
    }
}

void PlayerSpriteLayer::onTouchesEnded(const std::vector<Touch*>& touches, Event* event)
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    auto w = this->_sprite->getBoundingBox().size.width/2;
    auto h = this->_sprite->getBoundingBox().size.height/2;

    for( auto touch : touches )
    {
        auto xpos = this->touchToPoint(touch).x + this->touchOffset.x;
        auto ypos =  this->touchToPoint(touch).y + this->touchOffset.y;
        
        if( touch && touchOffset.x && touchOffset.y
           && (xpos > w && ypos > h && xpos < visibleSize.width - w && ypos < visibleSize.height - h))
        {

            // set the new sprite position
            this->_sprite->setPosition(this->touchToPoint(touch) + this->touchOffset);
            auto x = this->_sprite->getPositionX();

            // animate falling of the sprite
            this->_sprite->runAction(Sequence::create(MoveTo::create(0.5,Vec2(x, visibleSize.height/3)),
                                                      nullptr
                                                      ));

            // now check if the player is out of screen and bring it // back to screen
            if(this->_sprite->getPositionX() >= visibleSize.width)
                this->_sprite->runAction(Sequence::create(MoveTo::create(1,Vec2(this->_sprite->getContentSize().width, this->_sprite->getPositionY())), nullptr));
            else if(this->_sprite->getPositionX() <= 0)
               this->_sprite->runAction(Sequence::create(MoveTo::create(1,Vec2(visibleSize.width - this->_sprite->getContentSize().width, this->_sprite->getPositionY())), nullptr));
        }
    }
}
