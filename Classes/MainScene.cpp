#include "MainScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "CharacterReader.hpp"
#include "ObstacleReader.hpp"
#include "Constants.hpp"

USING_NS_CC;

using namespace cocostudio::timeline;

Scene* MainScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = MainScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool MainScene::init()
{
    /**  you can create scene with following comment code instead of using csb file.
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);
    
    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label, 1);

    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("HelloWorld.png");

    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    // add the sprite as a child to this layer
    this->addChild(sprite, 0);
    **/
    
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    CSLoader* instance = CSLoader::getInstance();
    instance->registReaderObject("CharacterReader", (ObjectFactory::Instance) CharacterReader::getInstance);
    instance->registReaderObject("ObstacleReader", (ObjectFactory::Instance) ObstacleReader::getInstance);
    
    auto rootNode = CSLoader::createNode("MainScene.csb");
    
    Size size = Director::getInstance()->getVisibleSize();
    rootNode->setContentSize(size);
    ui::Helper::doLayout(rootNode);
    
    this->backgournd = rootNode->getChildByName("back");
    this->character = this->backgournd->getChildByName<Character*>("character");
    this->character->setLocalZOrder(1);
    this->ground = this->backgournd->getChildByName<Sprite*>("ground");
    ground->setLocalZOrder(1);
    this->ground2 = this->backgournd->getChildByName<Sprite*>("ground2");
    ground2->setLocalZOrder(1);
    
    addChild(rootNode);

    this->scoreLabel = this->backgournd->getChildByName<cocos2d::ui::TextBMFont*>("scoreLabel");
    this->scoreLabel->setLocalZOrder(1);
    
    return true;
}

void MainScene::onEnter()
{
    Layer::onEnter();
    this->setupTouchHandling();
    this->scheduleUpdate();
    this->triggerReady();
}

void MainScene::update(float dt) {
    for (auto obstacle : this->obstacles) {
        obstacle->moveLeft(SCROLL_SPEED_X * dt);
    }
    
    for (auto obstacle : this->obstacles)
        for (Rect obstaclerect : obstacle->getRect())
            if (this->character->getRect().intersectsRect(obstaclerect) || this->character->touchGround)
                this->triggerGameOver();
    if (this->state == State::Playing) {
        this->ground->setPositionX(this->ground->getPositionX() - SCROLL_SPEED_X * dt);
        this->ground2->setPositionX(this->ground2->getPositionX() - SCROLL_SPEED_X * dt);
        if (this->ground->getPositionX() < 0 && this->ground->getPositionX() + SCROLL_SPEED_X * dt > 0)
            this->ground2->setPositionX(this->ground->getPositionX() + 288);
        if (this->ground2->getPositionX() < 0 && this->ground2->getPositionX() + SCROLL_SPEED_X * dt > 0)
            this->ground->setPositionX(this->ground2->getPositionX() + 288);
    }
    
    for (auto obstacle : this->obstacles) {
        float currentX = obstacle->getPositionX();
        float lastX = currentX + SCROLL_SPEED_X * dt;
        if (currentX < this->character->getPositionX() && lastX > this->character->getPositionX())
            setScore(++score);
    }
}

void MainScene::createObstacle(float dt) {
    Obstacle* obstacle = dynamic_cast<Obstacle*>(CSLoader::createNode("Obstacle.csb"));
    obstacle->setPosition(OBSTACLE_INIT_X, CCRANDOM_0_1() * (OBSTACLE_MAX_Y - OBSTACLE_MIN_Y) + OBSTACLE_MIN_Y);
    this->backgournd->addChild(obstacle);
    this->obstacles.pushBack(obstacle);
    
    if (this->obstacles.size() > OBSTACLE_LIMIT) {
        this->obstacles.front()->removeFromParent();
        this->obstacles.erase(this->obstacles.begin());
    }
}

void MainScene::setupTouchHandling() {
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = [&](Touch* touch, Event* event)
    {
        switch (this->state) {
            case State::Ready:
                this->tirggerPlaying();
            case State::Playing:
                this->character->jump();
                break;
            case State::GameOver:
                auto nextGameScene = MainScene::createScene();
                auto transition = TransitionFade::create(1.0f, nextGameScene);
                Director::getInstance()->replaceScene(transition);
                break;
        }
        return true;
    };
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
}

void MainScene::triggerReady(){
    this->state = State::Ready;
    this->score = 0;
    this->setScore(this->score);
    this->character->setisFlying(false);
}

void MainScene::tirggerPlaying() {
    this->state = State::Playing;
    this->character->setisFlying(true);
    this->schedule(CC_SCHEDULE_SELECTOR(MainScene::createObstacle), OBSTACLE_TIME_SPAN);
}

void MainScene::triggerGameOver() {
    this->state = State::GameOver;
    this->unscheduleAllCallbacks();
}

void MainScene::setScore(int score) {
    this->scoreLabel->setString(std::to_string(score));
}