#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "TouchableSpriteLayer.h"

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
	CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(
			"backgroundmusic.wav", true);

    
    ///////////////////////////////
    // calculate origin and width height of the screen
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();



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

	auto label = LabelTTF::create("Score: 00", "Arial", 24);

	// position the label on the center of the screen
	label->setPosition(
			Vec2(
					origin.x + visibleSize.width
							- label->getContentSize().width / 2,
					origin.y + visibleSize.height
							- label->getContentSize().height / 2));

	// add the label as a child to this layer
	this->addChild(label, 3);


    

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
	this->createScrollingBackground(visibleSize, origin);
    
    
    
    ////////////////////////////////
    // add the touchable actor layer
    auto layer = new TouchableSpriteLayer;
    this->addChild(layer, 2);
    layer->release(); // addChild() retained so we release
    

    
    ///////////////////////////////
    // Add Targets and Enemies/hurdles
//    _collectables = Vector<Sprite *>(2);
    

	return true;
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
//    
//    _grass = Sprite::create("grass.png");
//    _grass2 = Sprite::create("grass.png");

//    _tree = Sprite::create("tree.png");

	//-------------------------------------------------------
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
			visibleSize.width - _background3->getContentSize().width / 2,
			origin.y + _background3->getContentSize().height / 3); // hill

	auto rightCenPos = Vec2(
			visibleSize.width - _background3->getContentSize().width / 2
					- _background4->getContentSize().width / 2,
			origin.y + _background3->getContentSize().height / 3); // hill

//    auto botCenPos = Vec2(
//                          origin.x + visibleSize.width/2,
//                          origin.y ); // grass
//
//    auto botLeftPos = Vec2(
//                          origin.x + visibleSize.width/2,
//                          origin.y ); // grass
//
//    auto botRightPos = Vec2(
//                          origin.x,
//                          origin.y ); // grass

	//--------------------------------------------------------

	// 3) Determine relative movement speeds for foreground and background
	Vec2 cloudSpeed = Vec2(0.1, 0.1);
	Vec2 hillSpeed = Vec2(0.04, 0.04);
//    Vec2 grassSpeed = Vec2(0.3, 0.3);
//    Vec2 treeSpeed = Vec2(0.7, 0.7);

	// 4) Add children to ParallaxNode

	_backgroundNode->addChild(_background3, 0, hillSpeed, leftCenPos);
	_backgroundNode->addChild(_background1, 0, cloudSpeed, leftTopPos);
	_backgroundNode->addChild(_background4, 0, hillSpeed, rightCenPos);
	_backgroundNode->addChild(_background2, 0, cloudSpeed, rightTopPos);
	//    _backgroundNode->addChild(_grass, 0, grassSpeed, botCenPos);
//    _backgroundNode->addChild(_grass2, 0, grassSpeed, botCenPos);
//    _backgroundNode->addChild(_tree, 0, treeSpeed, rightCenPos);

	this->scheduleUpdate();
}

void HelloWorld::update(float dt) {
	// write the code here to continuously
	// scroll the background to the left
	Vec2 backgroundScrollVert = Vec2(-1000, 0);
	_backgroundNode->setPosition(
			Vec2(
					_backgroundNode->getPosition()
							+ Vec2(backgroundScrollVert * dt)));

	int safeOffset = 0;
//			Director::getInstance()->getVisibleSize().width
//			- _background1->getContentSize().width - 10;

	// now for infinite scrolling,
	// keep on adding background to right
	// hence making it circular scroll
	auto backGrounds = new Vector<Sprite*>(7);

	backGrounds->pushBack(_background1);
	backGrounds->pushBack(_background2);
	backGrounds->pushBack(_background3);
	backGrounds->pushBack(_background4);
//    backGrounds->pushBack(_grass);
//    backGrounds->pushBack(_grass2);
//    backGrounds->pushBack(_tree);

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

void HelloWorld::menuCloseCallback(Ref* pSender) {

//    Director::getInstance()->popToSceneStackLevel(1);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
	return;
#endif

	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}
