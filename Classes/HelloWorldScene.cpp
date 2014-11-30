#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "PlayerSpriteLayer.h"
#include "GameOverScene.h"
#include "CollisionDetection.h"
#include "MainMenuScene.h"
#include "Coordinates.h"



//const float hurdleCollisionRadius = 5.0f;



Scene* HelloWorld::createScene() {
    
    // 'scene' is an autorelease object
    auto scene = Scene::create();//WithPhysics();
//    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    
//    Size visibleSize = Director::getInstance()->getVisibleSize();

//    auto body = PhysicsBody::createEdgeBox(visibleSize, PHYSICSBODY_MATERIAL_DEFAULT, 3);
    
//    auto edgeNode = Node::create();
//    edgeNode->setPosition(Point(visibleSize.width/2,visibleSize.height/2));
//    edgeNode->setPhysicsBody(body);
    
//    scene->addChild(edgeNode);
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();
//    layer->setPhysicsWorld(scene->getPhysicsWorld());
    
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
    // calculate origin and width height of the screen
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();
    
    
    
    

    /////////////////////////
    // Backgrounds
    this->createScrollingBackground(visibleSize, origin);
    
    
    ///////////////////////////////
    // Add Targets and Enemies/hurdles
    this->createScrollingTargets(visibleSize, origin);
    
    ///////////////////////////////
    //	Add the background music in init().
    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("dreamy.mp3", true);
    
    
    ////////////////////////////////  ACTOR LAYER  ///////////////////////////////////
    // add the touchable actor layer
    actor = new PlayerSpriteLayer;
    this->addChild(actor, 2);
    actor->autorelease();
    _oldtint = actor->_originalTint;
    _playerCollisionRadius = actor->_collisionRadius;
    
    _lives = 3; // Set lives
    
    
    
    
    /////////////////////////////////
    // Pause screen
    /////////////////////////////////
    pausescreenbartop = LayerColor::create(Color4B(0,0,0,100), visibleSize.width, visibleSize.height/2);
    pausescreenbartop->setPosition(0, visibleSize.height);
    this->addChild(pausescreenbartop, 1);
    pausescreenbarbot = LayerColor::create(Color4B(0,0,0,100), visibleSize.width, visibleSize.height/2);
    pausescreenbarbot->setPosition(0, -pausescreenbarbot->getContentSize().height);
    this->addChild(pausescreenbarbot, 1);
    
    
    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.
    
    // add a "close" icon to exit the progress. it's an autorelease object
    _backItem = MenuItemImage::create("btn_back.png",
                                      "btn_back.png",
                                      CC_CALLBACK_1(HelloWorld::mainMenu, this));
    _backItem->setScale(0.7);
    _backItem->setPosition(Vec2(_backItem->getBoundingBox().size.width/2 + 10,
                                _backItem->getBoundingBox().size.height/2 + 10));
    
    _pauseItem = MenuItemImage::create("btn_pause.png",
                                       "btn_pause.png",
                                       CC_CALLBACK_1(HelloWorld::pauseGame, this));
    _pauseItem->setScale(0.7);
    _pauseItem->setPosition(
                            Vec2(
                                 origin.x + visibleSize.width
                                 - _pauseItem->getBoundingBox().size.width / 2 - 10,
                                 origin.y + _pauseItem->getBoundingBox().size.height / 2 + 10));
    
    _playItem = MenuItemImage::create("btn_play.png",
                                      "btn_play.png",
                                      CC_CALLBACK_1(HelloWorld::resumeGame, this));
    _playItem->setScale(0.7);
    _playItem->setPosition(
                           Vec2(
                                origin.x + visibleSize.width
                                - _playItem->getBoundingBox().size.width / 2 - 10,
                                origin.y + _playItem->getBoundingBox().size.height / 2 + 10));
    
    
    _playItem->setVisible(false);
    _backItem->setVisible(false);
    
    
    // create menu, it's an autorelease object
    auto menu = Menu::create(_pauseItem, _backItem, _playItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu,1);
    
    /////////////////////////
    // Label to show at pause screen
    /////////////////////////
    _label = Label::createWithSystemFont("Game Paused", "Arial", 24); //Click play button to start the game.
    _label->setPosition(
                        Vec2(origin.x + visibleSize.width / 2,
                             visibleSize.height - _label->getContentSize().height)); // position the label on the center of the screen
    this->addChild(_label, 3); // add the label as a child to this layer
    _label->setVisible(false);
    
    
    
    this->showHint();
    
    
    
    
    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(HelloWorld::onContactBegin, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);
    
    
    return true;
}


bool HelloWorld::onContactBegin(cocos2d::PhysicsContact &contact){
    PhysicsBody *a = contact.getShapeA()->getBody();
    PhysicsBody *b = contact.getShapeB()->getBody();
    
    // check if the bodies are collided
    if ((1 == a->getCollisionBitmask() && 2 == b->getCollisionBitmask()) || (2 == a->getCollisionBitmask() && 1 == b->getCollisionBitmask())) {
        CCLOG("collision has occurred");
    }
    
    return true;
}



void HelloWorld::update(float dt) {
    if (!isPaused) {
        this->updateBackground(dt);
        this->updateNuts(dt);
        if (shouldSpawn) {
            this->updateTargets(dt);
            this->updateCollectables(dt);
        }
    }
}



void HelloWorld::showHint(){
    ///////////////////////////////
    // add Hint image
    
    _hintSprite = Sprite::create("hints.png");
    
    // position the sprite on the center of the screen
    _hintSprite->setPosition(
                     Vec2(origin.x + visibleSize.width / 2,
                          origin.y + visibleSize.height / 2));
    
    _hintSprite->setScale(0.41);
    this->addChild(_hintSprite, 1);
    
    shouldSpawn = false;
    this->stopAllActions();
    this->runAction(Sequence::create(DelayTime::create(5),CallFunc::create(CC_CALLBACK_0(HelloWorld::hideHint, this)), NULL));
    
    // tap to hide
    // listen for touch events
    _listener = EventListenerTouchAllAtOnce::create();
    _listener->onTouchesBegan = CC_CALLBACK_2(self::onTouchesBegan, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(_listener, this);

}


void HelloWorld::onTouchesBegan(const std::vector<Touch*>& touches, Event* event)
{
    // reset touch offset
    this->touchOffset = Point::ZERO;
    for( auto touch : touches )
    {
        // if this touch is within our sprite's boundary
        if( touch && actor->isTouchingSprite(touch) )
        {
            hideHint();
        }
    }
}

void HelloWorld::hideHint(){
    this->getEventDispatcher()->removeEventListener(_listener);
    _hintSprite->setVisible(false);
    
    /////////////////////////////
    // add a top bar that shows "Score", collected coins and fruits
    this->createTopbarStats(visibleSize, origin);
    
    this->stopAllActions();
    this->scheduleUpdate();
    this->runAction(Sequence::create(DelayTime::create(5),CallFunc::create(CC_CALLBACK_0(HelloWorld::startSpawning, this)), NULL));
}

void HelloWorld::startSpawning(){
    shouldSpawn = true;
}


void HelloWorld::createScrollingBackground(cocos2d::Size visibleSize,
                                           cocos2d::Vec2 origin) {
    
    
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
    _grass = Sprite::create("roads.png");
    _grass->setPosition(visibleSize.width/2, 0);
    this->addChild(_grass, 0);
    
    ///////////////////////////////
    //Create the CCParallaxNode
    _backgroundNode = ParallaxNodeExtras::create(); //1
    this->addChild(_backgroundNode);
    
    // add "HelloWorld" sliding background
    _background1 = Sprite::create("cloud4.png");
    _background2 = Sprite::create("cloud5.png");
    
    _background3 = Sprite::create("mountain1.png");
    _background4 = Sprite::create("mountain2.png");
    
    _background3->setScale(0.5, 0.5);
    _background4->setScale(0.5, 0.5);
    
    
    _street3 = Sprite::create("street04.png");
    _street4 = Sprite::create("street05.png");
    _street5 = Sprite::create("street06.png");

    _street3->setScale(0.5);
    _street4->setScale(0.5);
    _street5->setScale(0.5);
    
    _street1 = Sprite::create("street07.png");
    _street2 = Sprite::create("street08.png");
//    _street3 = Sprite::create("street03.png");
//    _street4 = Sprite::create("street02.png");
//    _street5 = Sprite::create("street01.png");
    
//    _street1->setScale(2);
//    _street2->setScale(2);
    
//    _street1 = Sprite::create("streetdark.png");
//    _street2 = Sprite::create("streetdark.png");
//    _street3 = Sprite::create("streetdark.png");
    
//    _street1->setAnchorPoint(Vec2(0,0));
//    _street2->setAnchorPoint(Vec2(0,0));
//    _street3->setAnchorPoint(Vec2(0,0));
    
    
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
    Vec2 cloudSpeed = Vec2(0.12, 0.12);
    Vec2 hillSpeed = Vec2(0.02, 0.02);
    Vec2 houseSpeed = Vec2(0.1, 0.1);
    Vec2 streetSpeed = Vec2(0.2, 0.2);
    
    // 4) Add children to ParallaxNode
    
    _backgroundNode->addChild(_background1, 0, cloudSpeed, leftTopPos);
    _backgroundNode->addChild(_background3, 1, hillSpeed, leftCenPos);
    _backgroundNode->addChild(_background4, 1, hillSpeed, rightCenPos);
    _backgroundNode->addChild(_background2, 0, cloudSpeed, rightTopPos);
    
    _backgroundNode->addChild(_street1, 1, houseSpeed, Vec2(origin.x,
                                                            origin.y + _street1->getBoundingBox().size.height / 2));
    
    _backgroundNode->addChild(_street2, 1, houseSpeed, Vec2(origin.x+_street1->getBoundingBox().size.width,
                                                            origin.y + _street2->getBoundingBox().size.height / 2));
    
    _backgroundNode->addChild(_street3, 1, streetSpeed, Vec2(origin.x,
                                                             origin.y + _street3->getBoundingBox().size.height / 2));
    
    _backgroundNode->addChild(_street4, 1, streetSpeed, Vec2(origin.x+_street4->getBoundingBox().size.width,
                                                             origin.y + _street4->getBoundingBox().size.height / 2));
    
    _backgroundNode->addChild(_street5, 1, streetSpeed, Vec2(origin.x+_street4->getBoundingBox().size.width*2,
                                                             origin.y + _street5->getBoundingBox().size.height / 2));
    
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
    
    backGrounds->pushBack(_street1);
    backGrounds->pushBack(_street2);
    backGrounds->pushBack(_street3);
    backGrounds->pushBack(_street4);
    backGrounds->pushBack(_street5);
    
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





#define KNUMNUTS 10
#define KNUMHURDLES 1
#define KNUMPOWERUPS 1
#define KNUMLIVES 3
#define KNUMCOINS 10

void HelloWorld::createTopbarStats(cocos2d::Size visibleSize,
                                   cocos2d::Vec2 origin){
    
    _score = 00;
    _lives = KNUMLIVES;
    _nutscore = 0;
    _coins = 0;
    
    
    // add the label as a child to this layer
    //    _lscore = Label::createWithSystemFont("Score:\n" + std::to_string(_score), "Arial", 16);
    //    _lscore->setColor( Color3B(0, 0, 0) );
    //    // position the label on the center of the screen
    //    _lscore->setPosition(
    //                         Vec2(
    //                              origin.x + visibleSize.width
    //                              - _lscore->getContentSize().width / 2,
    //                              origin.y + visibleSize.height
    //                              - _lscore->getContentSize().height / 2));
    //    this->addChild(_lscore, 3);
    //    _lscore->setVisible(false);
    
    //    // add the label as a child to this layer
    //    _lcoins = Label::createWithSystemFont("Coins:\n" + std::to_string(_coins), "Arial", 16);
    //    _lcoins->setColor( Color3B(0, 0, 0) );
    //    // position the label on the center of the screen
    //    _lcoins->setPosition(
    //                         Vec2(
    //                              origin.x + visibleSize.width - _lscore->getContentSize().width
    //                              - _lnuts->getContentSize().width - _lcoins->getContentSize().width / 2 - 40,
    //                              origin.y + visibleSize.height
    //                              - _lcoins->getContentSize().height / 2));
    //    this->addChild(_lcoins, 3);
    
    // Nut board image
    auto nutboard = Sprite::create("nuts_score.png");
    nutboard->setScale(0.5f);
    nutboard->setPosition(
                          Vec2(
                               origin.x + visibleSize.width
                               - nutboard->getContentSize().width / 3,
                               origin.y + visibleSize.height
                               - nutboard->getContentSize().height / 3));
    this->addChild(nutboard);
    
    // add the label as a child to this layer
    _lnuts = Label::createWithSystemFont(std::to_string(_nutscore), "Arial", 16);
    _lnuts->setColor( Color3B(0, 0, 0) );
    // position the label on the center of the screen
    _lnuts->setPosition(
                        Vec2(
                             origin.x + visibleSize.width - 30
                             - _lnuts->getContentSize().width / 2 ,
                             origin.y + visibleSize.height - 15
                             - _lnuts->getContentSize().height / 2));
    this->addChild(_lnuts, 3);
    
    
    
    // heart icon
    auto heart = Sprite::create("heart.png");
    heart->setScale(0.3);
    heart->setPosition(
                       Vec2(
                            origin.x + heart->getBoundingBox().size.width / 2,
                            origin.y + visibleSize.height - heart->getBoundingBox().size.height/2));
    this->addChild(heart);
    
    // add the label as a child to this layer
    _llives = Label::createWithSystemFont(" x " + std::to_string(_lives), "Arial", 16);
    _llives->setColor( Color3B(0, 0, 0) );
    // position the label on the center of the screen
    _llives->setPosition(
                         Vec2(
                              origin.x + heart->getBoundingBox().size.width / 2 + _llives->getContentSize().width / 2 + 10,
                              origin.y + visibleSize.height
                              - _llives->getContentSize().height / 2 - 10));
    this->addChild(_llives, 3);
}






#define NUTS 0

enum HURDLES
{
    ENEMY = 1
};

enum POWERUPS
{
    SHEILD = 2,
    AMMO = 3,
    MAGNET = 4,
    SPEED = 5
};


void HelloWorld::createScrollingTargets(cocos2d::Size visibleSize,
                                        cocos2d::Vec2 origin) {
    
    
    /*******************
     0 = nuts
     1 = hurdles
     2 = powerups
     ******************/
    
    isSheilded = false; // initially no sheilds
    
    _collectables = new Vector<Sprite *>();
    _nuts = new Vector<Sprite *>();    
    _hurdles = new Vector<Sprite *>();
    
    for(int i = 0; i < KNUMNUTS; ++i) {
        auto nut = Sprite::create("nut.png");
        nut->setTag(NUTS);
        nut->setVisible(false);
        this->addChild(nut, 2);
        _nuts->pushBack(nut);
    }
    
    for(int i = 0; i < KNUMPOWERUPS; ++i) {
        auto sheild = Sprite::create("shield.png");
        sheild->setTag(SHEILD);
        sheild->setVisible(false);
        this->addChild(sheild, 1);
        _collectables->pushBack(sheild);
        
        auto ammo = Sprite::create("sun.png");
        ammo->setTag(AMMO);
        ammo->setVisible(false);
        this->addChild(ammo, 1);
        _collectables->pushBack(ammo);
    }
//    _nextCollectableSpawn = 90000; // we don't want power ups to appear immediately
    
    
    for(int i = 0; i < KNUMHURDLES; ++i) {
        auto bird = Sprite::create("birdAnim0000.png");
        bird->setTag(ENEMY);
        bird->setName("birdAnim00");
        bird->setVisible(false);
        bird->setFlippedX(true);
        
        auto birdbody = PhysicsBody::createCircle(bird->getBoundingBox().size.width/2);
        birdbody->setCollisionBitmask(2);
        birdbody->setContactTestBitmask(true);
        bird->setPhysicsBody(birdbody);
        
        this->addChild(bird, 3);
        _hurdles->pushBack(bird);
        
        
        auto bird2 = Sprite::create("birdAmin0000.png");
        bird2->setTag(ENEMY);
        bird2->setName("birdAmin00");
        bird2->setVisible(false);
        bird2->setFlippedX(true);
        
        auto birdbody2 = PhysicsBody::createCircle(bird2->getBoundingBox().size.width/2);
        birdbody2->setCollisionBitmask(2);
        birdbody2->setContactTestBitmask(true);
        bird2->setPhysicsBody(birdbody2);
        
        this->addChild(bird2, 3);
        _hurdles->pushBack(bird2);
    }
    
}










void HelloWorld::updateNuts(float dt){
    // Random generation of collectables and hurdles
    float curTimeMillis = getTimeTick();
    //////////////////////////////
    // Create NUTS
    //////////////////////////////////////
    
    if (curTimeMillis > _nextNutSpawn) {
        // time has come to spawn new nut
        float randMillisecs = randomValueBetween(2.0, 3.0) * 1000; // (0.20 , 1.0)
        _nextNutSpawn = randMillisecs + curTimeMillis;
        
        auto heightNut = ((Sprite *)_nuts->at(0))->getBoundingBox().size.height;
        auto widthNut = ((Sprite *)_nuts->at(0))->getBoundingBox().size.width;
        
        float randY = randomValueBetween(heightNut*2, visibleSize.height - heightNut*2); // bottom and top || (0 , maxheight)
        float randDuration =  2.0;//<--Lets use fixed duration for now || randomValueBetween(2.0,10.0);
        
        float randCount = randomValueBetween(3, 9); // for number of nuts only
        
        // now make a diamond / square / rectangle shaped pattern for nuts (spawn 5 at a time, for example)
        
        ///////////////////////////////////////////
        // first make rect for diamond of 9 cells
        // 0 = nut is not present
        // 1 = nut is present
        //////////////////////////////////////////
        
        /*************\
         *  abc  abc  *
         1  000  010  1
         2  000  111  2
         3  000  010  3
         *  abc  abc  *
         \*************/
        
        //////////////////////////////////////////////////////////////////////////////
        // Let's define positions
        
        int rows = 3;
        int cols = 3;
        
        auto cordinates = new Vector<Coordinates *>(randCount);
        
        for (int i = 0; i < rows; i++){
            randY = randY + heightNut * (i - 1); // only valid for 3 x 3 matrix
            for (int j = 0; j < cols; j++) {
                //
                auto cord = new Coordinates();
                cord->setPositon(visibleSize.width+widthNut*(j+1), randY); // optimise this section
                cordinates->pushBack(cord);
            }
        }
        
        // Keep generating nuts
        for (_nextNut = 0; _nextNut <= randCount; _nextNut++) {
            Sprite *nut = (Sprite *)_nuts->at(_nextNut);
            nut->stopAllActions();
            auto position = ((Coordinates* ) cordinates->at(_nextNut))->getPosition();
            nut->setPosition(position);
            nut->setVisible(true);
            nut->runAction(Sequence::create(
                                            MoveBy::create(randDuration, Vec2(-visibleSize.width-nut->getContentSize().width*cols, 0)),
                                            CallFuncN::create(CC_CALLBACK_1(HelloWorld::setInvisible, this)),
                                            NULL // DO NOT FORGET TO TERMINATE WITH NULL (unexpected in C++)
                                            ));
        } // end for //
        
    }
    
    ///////////////////------------------ COLLISION DETECTION ---------------------//////////////////

    // for each nuts, hide when touched
    for (auto nut: *_nuts) {
        if (!((Sprite *) nut)->isVisible() )
            continue;
        // pp collision detection
        if (CollisionDetection::getInstance()->collidesWithSpriteHavingRadius(
                                                                              actor->_sprite, _playerCollisionRadius, (Sprite *)nut, ((Sprite *)nut)->getBoundingBox().size.width/2)) {
            if (((Sprite *)nut)->getTag() == NUTS) { // nut
                // it's a nut
                _nutscore++;
                _lnuts->setString(std::to_string(_nutscore));
                
                // Now update the score.
                _score = (_nutscore)*2;
                //                _lscore->setString("Score:\n" + std::to_string(_score));
                CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("collect.wav");
                auto collectNut = ParticleExplosion::createWithTotalParticles(1);
                collectNut->setPosition(((Sprite *)nut)->getPosition());
                this->addChild(collectNut);
            }
            
            ((Sprite *) nut)->setVisible(false);
        }
    }
}
    
void HelloWorld::updateCollectables(float dt){
    // Random generation of collectables and hurdles
    float curTimeMillis = getTimeTick();
    /////////////////////////////////////////////////////////////////////////////////
    // Create collectables (like power ups)
    ///////////////////////////////////////////////////////////////////
    
    if (curTimeMillis > _nextCollectableSpawn) { // time has come to spawn new collectable
        
        float randMillisecs = randomValueBetween(5.0, 15.0) * 1000;
        _nextCollectableSpawn = randMillisecs + curTimeMillis;
        
        float randY = randomValueBetween(20 , visibleSize.height - 20);
        float randDuration = 2.0; // randomValueBetween(8.0, 5.0);
        
        Sprite *collectable = (Sprite *)_collectables->at(_nextCollectable);
        _nextCollectable++;
        
        if (_nextCollectable >= _collectables->size())
            _nextCollectable = 0;
        
        // In case of Sheild is already visible on the stage or equiped by the actor, do not appear
        if (collectable->getTag() == SHEILD && !isSheilded && !isSheildVisible) {
            collectable->stopAllActions();
            collectable->setPosition( Vec2(visibleSize.width+collectable->getContentSize().width/2, randY));
            collectable->setVisible(true);
            collectable->runAction(Sequence::create(
                                                    MoveBy::create(randDuration, Vec2(-visibleSize.width-collectable->getContentSize().width*3, 0)),
                                                    CallFuncN::create(CC_CALLBACK_1(HelloWorld::setInvisible, this)),
                                                    NULL // DO NOT FORGET TO TERMINATE WITH NULL (unexpected in C++)
                                                    ));//create(this, callfuncN_selector(HelloWorld::setInvisible)
        }
    }
    
    ///////////////////------------------ COLLISION DETECTION ---------------------//////////////////

    
    // for each powerups, start effect and hide when touched
    for (auto collectable: *_collectables) {
        if (!((Sprite *) collectable)->isVisible() )
            continue;
        // pp collision detection
        if (CollisionDetection::getInstance()->collidesWithSpriteHavingRadius(
                                                                              actor->_sprite, _playerCollisionRadius, (Sprite *)collectable, ((Sprite *)collectable)->getBoundingBox().size.width/2)){
            if (((Sprite *)collectable)->getTag() == SHEILD) { // power up
                // it's a sheild
                // _lives++;
                auto sheildedEffect = Sequence::create(TintTo::create(0.5, 256, 100, 100),
                                                       TintTo::create(0.5, _oldtint.r, _oldtint.g, _oldtint.b), NULL);
                
                actor->_sprite->runAction(
                                          Sequence::create(
                                                           Repeat::create(sheildedEffect, 10),
                                                           CallFunc::create(CC_CALLBACK_0(HelloWorld::removeSheildEffect, this)), NULL));
                isSheilded = true;
                CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("powerup.wav");
//                auto collectSheild = ParticleFire::createWithTotalParticles(4);
//                collectSheild->setPosition(((Sprite *)collectable)->getPosition());
//                this->addChild(collectSheild);
            }
            
            ((Sprite *) collectable)->setVisible(false);
        }
    }
}

void HelloWorld::updateTargets(float dt){
    // Random generation of collectables and hurdles
    float curTimeMillis = getTimeTick();
    
    ///////////////////////////////////////////////////////////////
    /// Create new hurdles
    ///////////////////////////////////////////////////////////////
    if (curTimeMillis > _nextHurdleSpawn) { // time has come to spawn new hurdles
        
        float randMillisecs = randomValueBetween(1.0 , 4.0) * 1000; // (0.20 , 1.0)
        _nextHurdleSpawn = randMillisecs + curTimeMillis;
        
        float randY = randomValueBetween(20 , visibleSize.height - 20);
        float randH = randomValueBetween(0 , visibleSize.height);
        float randDuration = randomValueBetween(4.0, 3.0);
        
        Sprite *hurdle = (Sprite *)_hurdles->at(_nextHurdle);
        _nextHurdle++;
        
        if (_nextHurdle >= _hurdles->size())
            _nextHurdle = 0;
        
        
        hurdle->stopAllActions();
        
        //---------------------------------
        auto spriteName = hurdle->getName();
        Vector<SpriteFrame*> animFrames(11);
        char str[100] = {0};
        for(int i = 1; i < 11; i++)
        {
            sprintf(str, "%02d.png",i);
            auto frame = SpriteFrame::create(spriteName + str,Rect(0,0,60,70)); //we assume that the sprites' dimentions are 60*70 rectangles.
            animFrames.pushBack(frame);
        }
        
        auto animation = Animation::createWithSpriteFrames(animFrames, 0.2f);
        animation->setLoops(-1);
        animation->setDelayPerUnit(0.05);
        auto animate = Animate::create(animation);
        hurdle->runAction(animate);
        //----------------------------------
        
        hurdle->setPosition( Vec2(visibleSize.width+hurdle->getContentSize().width/2, randY));
        hurdle->setVisible(true);
        hurdle->runAction(Sequence::create(MoveTo::create(randDuration, Vec2(origin.x - visibleSize.width, randH)),
                                           CallFuncN::create(CC_CALLBACK_1(HelloWorld::setInvisible, this)), NULL) // DO NOT FORGET TO TERMINATE WITH NULL (unexpected in C++)
                                                );
    }
    
    
    ///////////////////------------------ COLLISION DETECTION ---------------------//////////////////
    
//     for each hurdle, hit player
    for (auto hurdle : *_hurdles) {
        if (!((Sprite *) hurdle)->isVisible() )
            continue;
        
        // pp collision detection
        if (
//        CollisionDetection::getInstance()->collidesWithSprite(actor->_sprite, (Sprite *)hurdle, true))
            CollisionDetection::getInstance()->collidesWithSpriteHavingRadius(actor->_sprite, _playerCollisionRadius, (Sprite *)hurdle, ((Sprite *)hurdle)->getBoundingBox().size.width/2))
//            areTheSpritesColliding(actor->_sprite, (Sprite *)hurdle, true, _rt))
        {
//            if (((Sprite *)hurdle)->getTag() == 1){ // bird
                if (!isSheilded) {
                    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("pew.wav");
                    _lives--;
                    _llives->setString(" x " + std::to_string(_lives));
                    // game over!!
                    if (_lives == 0) {
                        // Show score & Game over scene with restart button
                        CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
                        this->runAction(Sequence::create(
                                                         CallFunc::create(CC_CALLBACK_0(HelloWorld::stopEverythingMoving, this)),
                                                         DelayTime::create(1),
                                                         CallFunc::create(CC_CALLBACK_0(HelloWorld::gameOver, this)), NULL));
                        break;
                    }
                    actor->_sprite->runAction(Sequence::createWithTwoActions(Blink::create(1.0, 9),
                                                                             CallFunc::create(CC_CALLBACK_0(HelloWorld::makeActorVisible, this))));
                }
                else {
                    // explode stars
                    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.wav");
                    auto destroyStar = ParticleExplosion::createWithTotalParticles(4);
                    destroyStar->setPosition(((Sprite *)hurdle)->getPosition());
                    this->addChild(destroyStar);
                    _score = _score+2;
                }
//            }
            
            ((Sprite *)hurdle)->setVisible(false);
        }
    }
}


void HelloWorld::removeSheildEffect() {
    isSheilded = false;
}


void HelloWorld::setInvisible(Node * node) {
    if(node->getTag() == SHEILD)
        isSheildVisible = false;
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






void HelloWorld::gameOver(){
    // stop music loop and play for game over
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("invalid.wav");
    
    // stop everything moving
//    actor->_sprite->stopAllActions();
    this->unscheduleUpdate();
    GameOverScene *gameOverScene = GameOverScene::create();
    gameOverScene->getLayer()->getLabel()->setString("0" + std::to_string(_nutscore));
    gameOverScene->getLayer()->_score->setString("Total Score: " + std::to_string(_score));
    
    auto transition = TransitionSplitRows::create(1.0f, gameOverScene);
    CCDirector::getInstance()->pushScene(transition);//replaceScene(transition); doesnot work in case of transition it gives error
}







void HelloWorld::mainMenu(Ref* pSender) {
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("explode.wav");
    CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    auto scene = MainMenu::createScene();
    CCDirector::getInstance()->pushScene(scene);
}




void HelloWorld::stopEverythingMoving(){
    CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    CocosDenshion::SimpleAudioEngine::getInstance()->pauseAllEffects();
    actor->pause();
    actor->_sprite->pause();
    
    for (int i = 0; i < _collectables->size(); i++) {
        _collectables->at(i)->pause();
    }
    
    for (int i = 0; i < _nuts->size(); i++) {
        _nuts->at(i)->pause();
    }
    
    for (int i = 0; i < _hurdles->size(); i++) {
        _hurdles->at(i)->pause();
    }
    isPaused = true;
}


void HelloWorld::pauseGame(Ref* pSender) {
    this->stopEverythingMoving();
    
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("explode.wav");
    
    _label->setVisible(true);
    
    // show resume button
    _pauseItem->setVisible(false);
    _playItem->setVisible(true);
    _backItem->setVisible(true);
    
    
    pausescreenbartop->runAction(Sequence::create(MoveTo::create(0.2,
                                                                 Vec2(0, visibleSize.height - pausescreenbartop->getContentSize().height)), NULL));
    pausescreenbarbot->runAction(MoveTo::create(0.2,
                                                Vec2(0, 0)));
    
    this->pause();
}

void HelloWorld::resumeGame(Ref* pSender) {
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("explode.wav");
    CocosDenshion::SimpleAudioEngine::getInstance()->resumeAllEffects();
    CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
    
    
    pausescreenbartop->runAction(MoveTo::create(0.5,
                                                Vec2(0, visibleSize.height)));
    pausescreenbarbot->runAction(MoveTo::create(0.5,
                                                Vec2(0, - pausescreenbarbot->getContentSize().height)));
    
    _label->setVisible(false);
    
    
    // show pause button
    _pauseItem->setVisible(true);
    _playItem->setVisible(false);
    _backItem->setVisible(false);
    
    actor->resume();
    actor->_sprite->resume();
    
    for (int i = 0; i < _collectables->size(); i++) {
        _collectables->at(i)->resume();
    }
    
    
    for (int i = 0; i < _nuts->size(); i++) {
        _nuts->at(i)->resume();
    }
    
    
    for (int i = 0; i < _hurdles->size(); i++) {
        _hurdles->at(i)->resume();
    }
    
    isPaused = false;
    this->resume();

}



void HelloWorld::makeActorVisible(){
    actor->_sprite->setVisible(true);
}









