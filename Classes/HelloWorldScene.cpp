#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "TouchableSpriteLayer.h"
#include "GameOverScene.h"
#include "CollisionDetection.h"
#include "MainMenuScene.h"


#define KNUMCOLLECTABLES 10
#define KNUMHURDLES 4
#define KNUMPOWERUPS 1
#define KNUMLIVES 3
#define KNUMCOINS 10


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
    // calculate origin and width height of the screen
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();
    
    
    
    
    
    /////////////////////////////////
    // Pause screen
    pausescreenbartop = LayerColor::create(Color4B(0,0,0,200), visibleSize.width, visibleSize.height/2);
    pausescreenbartop->setPosition(0, visibleSize.height);
    this->addChild(pausescreenbartop, 3);
    
    pausescreenbarbot = LayerColor::create(Color4B(0,0,0,200), visibleSize.width, visibleSize.height/2);
    pausescreenbarbot->setPosition(0, -pausescreenbarbot->getContentSize().height);
    this->addChild(pausescreenbarbot, 3);
    
    
    
    
    _label = Label::createWithSystemFont("Game Paused", "Arial", 24);
    
    // position the label on the center of the screen
    _label->setPosition(
                       Vec2(origin.x + visibleSize.width / 2,
                            visibleSize.height - _label->getContentSize().height));
    
    // add the label as a child to this layer
    this->addChild(_label, 3);
    
    _label->setVisible(false);
    
    
    
    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.
    
    // add a "close" icon to exit the progress. it's an autorelease object
    _backItem = MenuItemImage::create("btn_back.png",
                                          "btn_back.png",
                                          CC_CALLBACK_1(HelloWorld::mainMenu, this));
    _backItem->setScale(0.7);
    _backItem->setPosition(Vec2(_backItem->getContentSize().width/2 + 10, _backItem->getContentSize().height/2 + 10));
    
    _pauseItem = MenuItemImage::create("btn_pause.png",
                                           "btn_pause.png",
                                           CC_CALLBACK_1(HelloWorld::pauseGame, this));
    _pauseItem->setScale(0.7);
    _pauseItem->setPosition(
                           Vec2(
                                origin.x + visibleSize.width
                                - _pauseItem->getContentSize().width / 2 - 10,
                                origin.y + _pauseItem->getContentSize().height / 2 + 10));
    
    _playItem = MenuItemImage::create("btn_play.png",
                                           "btn_play.png",
                                           CC_CALLBACK_1(HelloWorld::resumeGame, this));
    _playItem->setScale(0.7);
    _playItem->setPosition(
                           Vec2(
                                origin.x + visibleSize.width
                                - _playItem->getContentSize().width / 2 - 10,
                                origin.y + _playItem->getContentSize().height / 2 + 10));
    
    
    _playItem->setVisible(false);
    _backItem->setVisible(false);
    
    // create menu, it's an autorelease object
    auto menu = Menu::create(_pauseItem, _backItem, _playItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 3);
    
    
    
    
    /////////////////////////////
    // add a top bar that shows "Score", collected coins and fruits
    this->createTopbarStats(visibleSize, origin);
    
    
    
    
    
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
    
    
    
    ////////////////////////////////
    // pixel perfect collision detection
    _rt = RenderTexture::create(visibleSize.width *2, visibleSize.height *2);
    _rt->setPosition(Vec2(visibleSize.width, visibleSize.height));
    _rt->retain();
    _rt->setVisible(false);
    
    
    //////////////////////////////////
    // Enable Touch Sensor
    // listen for touch events
    //    auto listener = EventListenerTouchAllAtOnce::create();
    //    listener->onTouchesBegan = CC_CALLBACK_2(self::onTouchesBegan, this);
    //    listener->onTouchesMoved = CC_CALLBACK_2(self::onTouchesMoved, this);
    //    listener->onTouchesEnded = CC_CALLBACK_2(self::onTouchesEnded, this);
    //    listener->onTouchesCancelled = CC_CALLBACK_2(self::onTouchesEnded, this);
    //    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
    
    ///////////////////////////////
    //	Add the background music in init().
    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("backgroundmusic.wav", true);
    return true;
}





void HelloWorld::update(float dt) {
    
    this->updateBackground(dt);
    this->updateTargets(dt);
}





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








void HelloWorld::createScrollingTargets(cocos2d::Size visibleSize,
                                        cocos2d::Vec2 origin) {
    
    
    /*******************
     0 = nuts
     1 = hurdles
     2 = collectables
     3 = powerups
     ******************/
    
    isSheilded = false; // initially no sheilds
    
    _collectables = new Vector<Sprite *>();
    _nuts = new Vector<Sprite *>();
    
    for(int i = 0; i < KNUMCOLLECTABLES; ++i) {
        auto nut = Sprite::create("nut.png");
        nut->setTag(0);
        nut->setVisible(false);
        this->addChild(nut, 2);
        _nuts->pushBack(nut);
    }
    
    for(int i = 0; i < KNUMHURDLES; ++i) {
        auto star = Sprite::create("star.png");
        star->setTag(1);
        star->setVisible(false);
        this->addChild(star, 2);
        _collectables->pushBack(star);
    }
    
    for(int i = 0; i < KNUMPOWERUPS; ++i) {
        auto sheild = Sprite::create("shield.png");
        sheild->setTag(3);
        sheild->setVisible(false);
        this->addChild(sheild, 1);
        _collectables->pushBack(sheild);
    }
}





void HelloWorld::updateTargets(float dt){
    // Random generation of collectables and hurdles
    float curTimeMillis = getTimeTick();
    
    
    if (curTimeMillis > _nextNutSpawn) { // time has come to spawn new collectable
        float randMillisecs = randomValueBetween(2.0, 5.0) * 1000; // (0.20 , 1.0)
        _nextNutSpawn = randMillisecs + curTimeMillis;
        
        auto heightNut = ((Sprite *)_nuts->at(0))->getBoundingBox().size.height;
        auto widthNut = ((Sprite *)_nuts->at(0))->getBoundingBox().size.width;
        
        float randY = randomValueBetween(heightNut*2, visibleSize.height - heightNut*2); // bottom and top || (0 , maxheight)
        float randDuration =  2.0;//<--Lets use fixed duration for now || randomValueBetween(2.0,10.0);
        
        float randCount = randomValueBetween(4, 9); // for number of nuts only
        
        // now make a diamond shaped pattern for nuts (spawn 5 at a time, for example)
        
        ///////////////////////////////////////////
        // first make rect for diamond of 9 cells
        // 0 = nut is not present
        // 1 = nut is present
        /**************
         *  abc  abc
         1  000  010
         2  000  111
         3  000  010
         *
         **************/
        // Let's define positions
        //////////////////////////
        auto a1 = Vec2(visibleSize.width+widthNut, randY + heightNut);
        auto b1 = Vec2(visibleSize.width+widthNut*2, randY + heightNut);
        auto c1 = Vec2(visibleSize.width+widthNut*3, randY + heightNut);
        
        auto a2 = Vec2(visibleSize.width+widthNut, randY);
        auto b2 = Vec2(visibleSize.width+widthNut*2, randY);
        auto c2 = Vec2(visibleSize.width+widthNut*3, randY);
        
        auto a3 = Vec2(visibleSize.width+widthNut, randY - heightNut);
        auto b3 = Vec2(visibleSize.width+widthNut*2, randY - heightNut);
        auto c3 = Vec2(visibleSize.width+widthNut*3, randY - heightNut);
        
        
//        Vector<Vec2> positions(9);
//        positions.pushBack(a1);
//        positions.pushBack(a2);
//        positions.pushBack(a3);
//        
//        positions.pushBack(b1);
//        positions.pushBack(b2);
//        positions.pushBack(b3);
//        
//        positions.pushBack(c1);
//        positions.pushBack(c2);
//        positions.pushBack(c3);
        
        _nextNut = 0;
        
//        for (int i = 0; i < randCount; i++) {
        
        // keep generating nuts
        Sprite *collectable1 = (Sprite *)_nuts->at(_nextNut);
        _nextNut++;
        collectable1->stopAllActions();
        collectable1->setPosition(b1);
        collectable1->setVisible(true);
        collectable1->runAction(Sequence::create(
                                                MoveBy::create(randDuration, Vec2(-visibleSize.width-collectable1->getContentSize().width, 0)),
                                                CallFuncN::create(CC_CALLBACK_1(HelloWorld::setInvisible, this)),
                                                NULL // DO NOT FORGET TO TERMINATE WITH NULL (unexpected in C++)
                                                ));
        
        Sprite *collectable2 = (Sprite *)_nuts->at(_nextNut);
        _nextNut++;
        collectable2->stopAllActions();
        collectable2->setPosition(a2);
        collectable2->setVisible(true);
        collectable2->runAction(Sequence::create(
                                                MoveBy::create(randDuration, Vec2(-visibleSize.width-collectable1->getContentSize().width, 0)),
                                                CallFuncN::create(CC_CALLBACK_1(HelloWorld::setInvisible, this)),
                                                NULL // DO NOT FORGET TO TERMINATE WITH NULL (unexpected in C++)
                                                ));
        
        Sprite *collectable3 = (Sprite *)_nuts->at(_nextNut);
        _nextNut++;
        collectable3->stopAllActions();
        collectable3->setPosition(b2);
        collectable3->setVisible(true);
        collectable3->runAction(Sequence::create(
                                                MoveBy::create(randDuration, Vec2(-visibleSize.width-collectable1->getContentSize().width, 0)),
                                                CallFuncN::create(CC_CALLBACK_1(HelloWorld::setInvisible, this)),
                                                NULL // DO NOT FORGET TO TERMINATE WITH NULL (unexpected in C++)
                                                ));
        
        Sprite *collectable4 = (Sprite *)_nuts->at(_nextNut);
        _nextNut++;
        collectable4->stopAllActions();
        collectable4->setPosition(b3);
        collectable4->setVisible(true);
        collectable4->runAction(Sequence::create(
                                                MoveBy::create(randDuration, Vec2(-visibleSize.width-collectable1->getContentSize().width, 0)),
                                                CallFuncN::create(CC_CALLBACK_1(HelloWorld::setInvisible, this)),
                                                NULL // DO NOT FORGET TO TERMINATE WITH NULL (unexpected in C++)
                                                ));
        
        Sprite *collectable5 = (Sprite *)_nuts->at(_nextNut);
        _nextNut++;
        collectable5->stopAllActions();
        collectable5->setPosition(c2);
        collectable5->setVisible(true);
        collectable5->runAction(Sequence::create(
                                                MoveBy::create(randDuration, Vec2(-visibleSize.width-collectable1->getContentSize().width, 0)),
                                                CallFuncN::create(CC_CALLBACK_1(HelloWorld::setInvisible, this)),
                                                NULL // DO NOT FORGET TO TERMINATE WITH NULL (unexpected in C++)
                                                ));
        
        _nextNut = 0;
        
        
//        } // end for //
    }
        
        
    if (curTimeMillis > _nextCollectableSpawn) { // time has come to spawn new collectable


        float randMillisecs = randomValueBetween(0.20 , 1.0) * 1000; // (0.20 , 1.0)
        _nextCollectableSpawn = randMillisecs + curTimeMillis;

        float randY = randomValueBetween(20 , visibleSize.height - 20);
        float randDuration =  randomValueBetween(8.0, 5.0);

        Sprite *collectable = (Sprite *)_collectables->at(_nextCollectable);
        _nextCollectable++;

        if (_nextCollectable >= _collectables->size())
        _nextCollectable = 0;

        collectable->stopAllActions();
        collectable->setPosition( Vec2(visibleSize.width+collectable->getContentSize().width/2, randY));
        collectable->setVisible(true);
        collectable->runAction(Sequence::create(
                                    MoveBy::create(randDuration, Vec2(-visibleSize.width-collectable->getContentSize().width, 0)),
                                    CallFuncN::create(CC_CALLBACK_1(HelloWorld::setInvisible, this)),
                                    NULL // DO NOT FORGET TO TERMINATE WITH NULL (unexpected in C++)
                                    ));//create(this, callfuncN_selector(HelloWorld::setInvisible)
    }
    
    
    ///////////////////--------------------------------------------------//////////////////
    
    // for each nuts, hide when touched
    for (auto nut: *_nuts) {
        if (!((Sprite *) nut)->isVisible() )
            continue;
        // pp collision detection
        if (CollisionDetection::GetInstance()->areTheSpritesColliding(actor->_sprite, (Sprite *)nut, false, _rt)) {
            if (((Sprite *)nut)->getTag() == 0) { // nut
                // it's a nut
                _nutscore++;
                _lnuts->setString(std::to_string(_nutscore));
                
                // Now update the score.
                _score = (_nutscore)*2;
                //                _lscore->setString("Score:\n" + std::to_string(_score));
                CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("collect.wav");
            }
            
            ((Sprite *) nut)->setVisible(false);
        }
    }
    
    // for each collectables,hide when touched
    for (auto collectable : *_collectables) {
        if (!((Sprite *) collectable)->isVisible() )
            continue;
        
        // pp collision detection
        if (CollisionDetection::GetInstance()->areTheSpritesColliding(actor->_sprite, (Sprite *)collectable, false, _rt)) {
            if (((Sprite *)collectable)->getTag() == 1){ // hurdle
                if (!isSheilded) {
                    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("pew.wav");
                    actor->_sprite->runAction( Blink::create(1.0, 9));
                    _lives--;
                    _llives->setString(" x " + std::to_string(_lives));
                    // game over!!
                    if (_lives == 0) {
                        // Show score & Game over scene with restart button
                        this->gameOver();
                        break;
                    }
                }
                else {
                    // explode stars
                    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.wav");
                    auto destroyStar = ParticleExplosion::createWithTotalParticles(4);
                    destroyStar->setPosition(((Sprite *)collectable)->getPosition());
                    this->addChild(destroyStar);
                    _score = _score+2;
                }
            }
            if (((Sprite *)collectable)->getTag() == 3) { // power up
                // it's a sheild
                //                    _lives++;
                auto oldtint = actor->_sprite->getColor();
                auto sheildedEffect = Sequence::create(TintTo::create(0.5, 256, 100, 100),
                                                       TintTo::create(0.5, oldtint.r, oldtint.g, oldtint.b), NULL);
                
                actor->_sprite->runAction(
                                          Sequence::create(
                                                           Repeat::create(sheildedEffect, 10),
                                                           CallFunc::create(CC_CALLBACK_0(HelloWorld::removeSheildEffect, this)), NULL));
                isSheilded = true;
                CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("powerup.wav");
            }
            ((Sprite *)collectable)->setVisible(false);
        }
    }
}


void HelloWorld::removeSheildEffect() {
    isSheilded = false;
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






void HelloWorld::gameOver(){
    // stop music loop and play for game over
    CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("gameover.wav");
    
    // stop everything moving
    actor->_sprite->stopAllActions();
    this->unscheduleUpdate();
    GameOverScene *gameOverScene = GameOverScene::create();
    gameOverScene->getLayer()->getLabel()->setString(" x 0" + std::to_string(_score));
    
    auto transition = TransitionSplitRows::create(1.0f, gameOverScene);
    CCDirector::getInstance()->pushScene(transition);//replaceScene(transition);
}







void HelloWorld::mainMenu(Ref* pSender) {
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("explode.wav");
    CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    auto scene = MainMenu::createScene();
    CCDirector::getInstance()->replaceScene(scene);
}

void HelloWorld::pauseGame(Ref* pSender) {
    CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("explode.wav");
    
    
    _label->setVisible(true);
    
    // show resume button
    _pauseItem->setVisible(false);
    _playItem->setVisible(true);
    _backItem->setVisible(true);
    
    actor->pause();
    actor->_sprite->pause();
    
    for (int i = 0; i < _collectables->size(); i++) {
        _collectables->at(i)->pause();
    }
    
    for (int i = 0; i < _nuts->size(); i++) {
        _nuts->at(i)->pause();
    }
    
    this->pause();
    
    pausescreenbartop->runAction(Sequence::create(MoveTo::create(0.2,
                                                                 Vec2(0, visibleSize.height - pausescreenbartop->getContentSize().height)), NULL));
    pausescreenbarbot->runAction(MoveTo::create(0.2,
                                                Vec2(0, 0)));
}

void HelloWorld::resumeGame(Ref* pSender) {
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("explode.wav");
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
    
    this->resume();
}







/********************************************/




//Point HelloWorld::touchToPoint(Touch* touch)
//{
//    // convert the touch object to a position in our cocos2d space
//    return CCDirector::getInstance()->convertToGL(touch->getLocationInView());
//}
//
//bool HelloWorld::isTouchingSprite(Touch* touch)
//{
//    // here's one way, but includes the rectangular white space around our sprite
//    //return CGRectContainsPoint(this->sprite->boundingBox(), this->touchToPoint(touch));
//
//    // this way is more intuitive for the user because it ignores the white space.
//    // it works by calculating the distance between the sprite's center and the touch point,
//    // and seeing if that distance is less than the sprite's radius
//        return (this->_sprite->getPosition().getDistance(this->touchToPoint(touch)) < 100.0f);
//}
//
//void HelloWorld::onTouchesBegan(const std::vector<Touch*>& touches, Event* event)
//{
//    // reset touch offset
//    this->touchOffset = Point::ZERO;
//
//    for( auto touch : touches )
//    {
//        // if this touch is within our sprite's boundary
//        if( touch && this->isTouchingSprite(touch) )
//        {
//            // calculate offset from sprite to touch point
//            //            this->touchOffset = this->_sprite->getPosition() - this->touchToPoint(touch);
//
//            //            this->_sprite->setScale(1.0f);
//            //
//            //            // animate letting go of the sprite
//            //            this->_sprite->runAction(Sequence::create(
//            //                                                      ScaleBy::create(0.125f, 1.111f),
//            //                                                      ScaleBy::create(0.125f, 0.9f),
//            //                                                      nullptr
//            //                                                      ));
//        }
//    }
//}
//
//void HelloWorld::onTouchesMoved(const std::vector<Touch*>& touches, Event* event)
//{
//    for( auto touch : touches )
//    {
//        // set the new sprite position
//                if( touch && touchOffset.x && touchOffset.y )
//                    this->_sprite->setPosition(this->touchToPoint(touch) + this->touchOffset);
//    }
//}
//
//void HelloWorld::onTouchesEnded(const std::vector<Touch*>& touches, Event* event)
//{
//    //    Size visibleSize = Director::getInstance()->getVisibleSize();
//    Vec2 origin = Director::getInstance()->getVisibleOrigin();
//
//    for( auto touch : touches )
//    {
//        if( touch && touchOffset.x && touchOffset.y  )
//        {
//            //            //            CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
//            //
//            //            // set the new sprite position
//            //            this->_sprite->setPosition(this->touchToPoint(touch) + this->touchOffset);
//            //
//            //            // stop any existing actions and reset the scale
//            //            //            this->_sprite->stopAllActions();
//            //
//            //            auto x = this->_sprite->getPositionX();
//            //            auto h  = this->_sprite->getBoundingBox().size.height;
//            //
//            //            // animate falling of the sprite
//            //            this->_sprite->runAction(Sequence::create(MoveTo::create(0.5,Vec2(x, h/2)),
//            //                                                      nullptr
//            //                                                      ));
//            //
//            //
//            //            //	And play the sound effect in ccTouchesEnded() when the bullet is fired.
//            //
//            //            // cpp with cocos2d-x
//            //            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(
//            //                                                                        "fall.wav");
//        }
//    }
//}














