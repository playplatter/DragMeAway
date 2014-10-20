#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "TouchableSpriteLayer.h"
#include "GameOverScene.h"

Scene* HelloWorld::createScene() {
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}







// on "init" you need to initialize your instance
bool HelloWorld::init() {
    
    
    //////////////////////////////
    // 1. super init first
    if (!Layer::init()) {
        return false;
    }
    
    
    ///////////////////////////////
    //	Add the background music in init().
    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("backgroundmusic.wav", true);
    
    
    ///////////////////////////////
    // calculate origin and width height of the screen
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();
    
    
    
    
    
    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.
    
    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create("CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
    closeItem->setPosition(
                           Vec2(
                                origin.x + visibleSize.width
                                - closeItem->getContentSize().width / 2,
                                origin.y + closeItem->getContentSize().height / 2));
    
    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
    
    
    
    
    /////////////////////////////
    // add a label that shows "Score"
    _score = 00;
    _label = Label::createWithSystemFont("Score: 00", "Arial", 24);
    
    // position the label on the center of the screen
    _label->setPosition(
                        Vec2(
                             origin.x + visibleSize.width
                             - _label->getContentSize().width / 2,
                             origin.y + visibleSize.height
                             - _label->getContentSize().height / 2));
    
    // add the label as a child to this layer
    this->addChild(_label, 3);
    
    
    
    
    ///////////////////////////////
    // add static background image
    auto sky = Sprite::create("sky.png");
    
    // position the sprite on the center of the screen
    sky->setPosition(
                     Vec2(origin.x + visibleSize.width / 2,
                          origin.y + visibleSize.height / 2));
    
    this->addChild(sky, -1);
    
    
    
    
    //////////////////////////////
    // add a scrolling background
    _grass = Sprite::create("Roads.png");
    _grass->setPosition(visibleSize.width/2, 0);
    this->addChild(_grass, 0);
    
    this->createScrollingBackground(visibleSize, origin);
    
    
    
    ////////////////////////////////
    // add the touchable actor layer
    actor = new TouchableSpriteLayer;
    this->addChild(actor, 2);
    actor->release(); // addChild() retained so we release
    
    
    
    ////////////////////////////////
    // Set lives
    _lives = 3;
    
    
    ///////////////////////////////
    // Add Targets and Enemies/hurdles
    this->createScrollingTargets(visibleSize, origin);
    
    
    
    
    //////////////////////////////////
    // Enable Touch Sensor
    // listen for touch events
    auto listener = EventListenerTouchAllAtOnce::create();
    listener->onTouchesBegan = CC_CALLBACK_2(self::onTouchesBegan, this);
    listener->onTouchesMoved = CC_CALLBACK_2(self::onTouchesMoved, this);
    listener->onTouchesEnded = CC_CALLBACK_2(self::onTouchesEnded, this);
    listener->onTouchesCancelled = CC_CALLBACK_2(self::onTouchesEnded, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
    return true;
}





void HelloWorld::update(float dt) {
    
    this->updateBackground(dt);
    this->updateTargets(dt);
}








void HelloWorld::createScrollingTargets(cocos2d::Size visibleSize,
                                        cocos2d::Vec2 origin) {

#define KNUMCOLLECTABLES 15
#define KNUMHURDLES 8
    
    
    /*******************
     0 = collectables
     1 = hurdles
     2 = coins
     ******************/
    
    
    _collectables = new Vector<Sprite *>();
    
    for(int i = 0; i < KNUMCOLLECTABLES; ++i) {
        auto cherry = Sprite::create("cherry.png");
        auto pear = Sprite::create("pear.png");
        auto grapes = Sprite::create("grapes.png");
        
        cherry->setTag(0);
        pear->setTag(0);
        grapes->setTag(0);
        
        cherry->setVisible(false);
        this->addChild(cherry, 2);
        pear->setVisible(false);
        this->addChild(pear, 2);
        grapes->setVisible(false);
        this->addChild(grapes, 2);
        
        _collectables->pushBack(cherry);
        _collectables->pushBack(pear);
        _collectables->pushBack(grapes);
        
        auto star = Sprite::create("star.png");
        star->setTag(1);
        
        star->setVisible(false);
        this->addChild(star, 2);
        
        _collectables->pushBack(star);
    }
    
//    for(int i = 0; i < KNUMHURDLES; ++i) {
//        auto star = Sprite::create("star.png");
//        star->setTag(1);
//        
//        star->setVisible(false);
//        this->addChild(star, 2);
//        
//        _collectables->pushBack(star);
//    }
}





void HelloWorld::createScrollingBackground(cocos2d::Size visibleSize,
                                           cocos2d::Vec2 origin) {
    
    //Create the CCParallaxNode
    _backgroundNode = ParallaxNodeExtras::create(); //1
    this->addChild(_backgroundNode, -1);
    
    // add "HelloWorld" sliding background
    _background1 = Sprite::create("cloud4.png");
    _background2 = Sprite::create("cloud5.png");
    
    _background3 = Sprite::create("mountain1.png");
    _background4 = Sprite::create("mountain2.png");
    
    _background3->setScale(0.5, 0.5);
    _background4->setScale(0.5, 0.5);
    
    
    
    auto centerPos = Vec2(origin.x + visibleSize.width / 2,
                          origin.y + visibleSize.height / 2); // actor
    
    auto leftTopPos = Vec2(
                           origin.x + visibleSize.width
                           - _background1->getContentSize().width / 2, // cloud
                           origin.y + visibleSize.height);
    
    auto rightTopPos = Vec2(
                            visibleSize.width - _background1->getContentSize().width
                            - _background2->getContentSize().width / 2, // cloud
                            origin.y + visibleSize.height);
    
    auto leftCenPos = Vec2(
                           _background3->getContentSize().width / 2,
                           origin.y + _background3->getContentSize().height / 3 - _grass->getContentSize().height/3); // hill
    
    auto rightCenPos = Vec2(
                            visibleSize.width - _background4->getContentSize().width / 2
                            - _background4->getContentSize().width / 2,
                            origin.y + _background4->getContentSize().height / 3 - _grass->getContentSize().height/3); // hill
    
    
    
    
    // 3) Determine relative movement speeds for foreground and background
    Vec2 cloudSpeed = Vec2(0.06, 0.06);
    Vec2 hillSpeed = Vec2(0.02, 0.02);
    
    
    // 4) Add children to ParallaxNode
    
    _backgroundNode->addChild(_background1, 0, cloudSpeed, leftTopPos);
    _backgroundNode->addChild(_background2, 2, cloudSpeed, rightTopPos);
    _backgroundNode->addChild(_background3, 1, hillSpeed, leftCenPos);
    _backgroundNode->addChild(_background4, 1, hillSpeed, rightCenPos);
    
    this->scheduleUpdate();
}







void HelloWorld::updateBackground(float dt){
    // write the code here to continuously
    // scroll the background to the left
    Vec2 backgroundScrollVert = Vec2(-1000, 0);
    _backgroundNode->setPosition(
                                 Vec2(
                                      _backgroundNode->getPosition()
                                      + Vec2(backgroundScrollVert * dt)));
    
    int safeOffset = 0;
    
    // now for infinite scrolling,
    // keep on adding background to right
    // hence making it circular scroll
    auto backGrounds = new Vector<Sprite*>(4);
    
    backGrounds->pushBack(_background1);
    backGrounds->pushBack(_background2);
    backGrounds->pushBack(_background3);
    backGrounds->pushBack(_background4);
    
    for (auto background : *backGrounds) {
        float size = background->getContentSize().width;
        float xPosition = _backgroundNode->convertToWorldSpace(
                                                               background->getPosition()).x - (size / 2);
        
        if (xPosition - safeOffset < -size) {
            _backgroundNode->incrementOffset(
                                             Point(background->getContentSize().width * 2, 0),
                                             background);
        }
    }
}





void HelloWorld::updateTargets(float dt){
    // Empty
    float curTimeMillis = getTimeTick();
    if (curTimeMillis > _nextCollectableSpawn) {
        
        float randMillisecs = randomValueBetween(0.20,1.0) * 1000;
        _nextCollectableSpawn = randMillisecs + curTimeMillis;
        
        float randY = randomValueBetween(0.0,visibleSize.height);
        float randDuration = randomValueBetween(2.0,10.0);
        
        Sprite *collectable = (Sprite *)_collectables->at(_nextCollectable);
        _nextCollectable++;
        
        if (_nextCollectable >= _collectables->size())
            _nextCollectable = 0;
        
        collectable->stopAllActions();
        collectable->setPosition( Vec2(visibleSize.width+collectable->getContentSize().width/2, randY));
        collectable->setVisible(true);
        collectable->runAction(Sequence::create(MoveBy::create(randDuration, Vec2(-visibleSize.width-collectable->getContentSize().width, 0)), CallFuncN::create(this, callfuncN_selector(HelloWorld::setInvisible)),
                                                NULL // DO NOT FORGET TO TERMINATE WITH NULL (unexpected in C++)
                                                ));
    }
    
    // for each collectables,hide when touched
    for (auto collectable : *_collectables) {
        if (!((Sprite *) collectable)->isVisible() )
            continue;
        if ((actor->_sprite)->boundingBox().intersectsRect(((Sprite *)collectable)->boundingBox()) ) {
            if (((Sprite *)collectable)->getTag() == 1){
                actor->_sprite->runAction( CCBlink::create(1.0, 9));
                _lives--; // game over!!
                if (_lives == 0) {
                    // Show score & Game over scene with restart button
                	CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
                    GameOverScene *gameOverScene = GameOverScene::create();
                    gameOverScene->getLayer()->getLabel()->setString("Game Over");
                    CCDirector::getInstance()->replaceScene(gameOverScene);
                }
            } else {
                // Now update the score.
                _score = _score + 1;
                _label->setString("Score: " + std::to_string(_score));
            }
            ((Sprite *)collectable)->setVisible(false);
        }
    }
}


void HelloWorld::setInvisible(Node * node) {
    node->setVisible(false);
}


float HelloWorld::randomValueBetween(float low, float high) {
    return (((float) arc4random() / 0xFFFFFFFFu) * (high - low)) + low;
}

float HelloWorld::getTimeTick() {
    timeval time;
    gettimeofday(&time, NULL);
    unsigned long millisecs = (time.tv_sec * 1000) + (time.tv_usec/1000);
    return (float) millisecs;
}












Point HelloWorld::touchToPoint(Touch* touch)
{
    // convert the touch object to a position in our cocos2d space
    return CCDirector::getInstance()->convertToGL(touch->getLocationInView());
}

bool HelloWorld::isTouchingSprite(Touch* touch)
{
    // here's one way, but includes the rectangular white space around our sprite
    //return CGRectContainsPoint(this->sprite->boundingBox(), this->touchToPoint(touch));
    
    // this way is more intuitive for the user because it ignores the white space.
    // it works by calculating the distance between the sprite's center and the touch point,
    // and seeing if that distance is less than the sprite's radius
    //    return (this->_sprite->getPosition().getDistance(this->touchToPoint(touch)) < 100.0f);
}

void HelloWorld::onTouchesBegan(const std::vector<Touch*>& touches, Event* event)
{
    // reset touch offset
    this->touchOffset = Point::ZERO;
    
    for( auto touch : touches )
    {
        // if this touch is within our sprite's boundary
        if( touch && this->isTouchingSprite(touch) )
        {
            // calculate offset from sprite to touch point
            //            this->touchOffset = this->_sprite->getPosition() - this->touchToPoint(touch);
            
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

void HelloWorld::onTouchesMoved(const std::vector<Touch*>& touches, Event* event)
{
    for( auto touch : touches )
    {
        // set the new sprite position
        //        if( touch && touchOffset.x && touchOffset.y )
        //            this->_sprite->setPosition(this->touchToPoint(touch) + this->touchOffset);
    }
}

void HelloWorld::onTouchesEnded(const std::vector<Touch*>& touches, Event* event)
{
    //    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    for( auto touch : touches )
    {
        if( touch && touchOffset.x && touchOffset.y  )
        {
            //            //            CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
            //
            //            // set the new sprite position
            //            this->_sprite->setPosition(this->touchToPoint(touch) + this->touchOffset);
            //
            //            // stop any existing actions and reset the scale
            //            //            this->_sprite->stopAllActions();
            //
            //            auto x = this->_sprite->getPositionX();
            //            auto h  = this->_sprite->getBoundingBox().size.height;
            //
            //            // animate falling of the sprite
            //            this->_sprite->runAction(Sequence::create(MoveTo::create(0.5,Vec2(x, h/2)),
            //                                                      nullptr
            //                                                      ));
            //            
            //            
            //            //	And play the sound effect in ccTouchesEnded() when the bullet is fired.
            //            
            //            // cpp with cocos2d-x
            //            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(
            //                                                                        "fall.wav");
        }
    }
}


















void HelloWorld::menuCloseCallback(Ref* pSender) {
    
//        Director::getInstance()->replaceScene(scene);
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
    MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif
    
    Director::getInstance()->end();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
