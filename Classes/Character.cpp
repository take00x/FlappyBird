//
//  Character.cpp
//  FlappyBird
//
//  Created by Takehiro Yoshihama on 2016/05/11.
//
//

#include "Character.hpp"
#include "Constants.hpp"

using namespace cocos2d;

bool Character::init()
{
    if (! Node::init())
    {
        return false;
    }
    
    // キャラクターのアニメーションのタイムラインを読み込む
    this->timeline = CSLoader::createTimeline("Character.csb");
    
    // キャラクターのアニメーションのタイムラインを確保し、メモリ領域が解放されないようにする
    this->timeline->retain();
    
    this->velocity = 0;
    this->accel = GRAVITY_ACCEL;
    this->isFlying = false;
    this->touchGround = false;
    
    return true;
}

void Character::update(float dt) {
    if (isFlying) {
        if (this->velocity >= SPEED_LIMIT)
            this->velocity += accel * dt;
        this->setPosition(this->getPosition() + Vec2(0, this->velocity * dt));
        if (this->getPositionY() - this->getChildByName("bird")->getContentSize().height / 2 <= character_min_y) {
            this->setPositionY(character_min_y + this->getChildByName("bird")->getContentSize().height / 2);
            this->touchGround = true;
        }
    }
}

void Character::onEnter() {
    Node::onEnter();
    this->scheduleUpdate();
    this->character_min_y = this->getParent()->getChildByName<Sprite*>("ground")->getContentSize().height;
}

void Character::jump() {
    this->velocity = JUMP_SPEED;
    
    this->stopAllActions();
    this->runAction(this->timeline);
    this->timeline->play("fly", false);
}

Rect Character::getRect() {
    float width = this->getChildByName("bird")->getContentSize().width;
    float height = this->getChildByName("bird")->getContentSize().height;
    return Rect(this->getPositionX() - width / 2, this->getPositionY() - height / 2, width, height);
}

void Character::setisFlying(bool isFlying) {
    this->isFlying = isFlying;
}