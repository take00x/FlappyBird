//
//  Character.cpp
//  FlappyBird
//
//  Created by Takehiro Yoshihama on 2016/05/11.
//
//

#include "Character.hpp"
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
    this->accel = -1000;
    
    return true;
}

void Character::update(float dt) {
    if (this->velocity > -400)
        this->velocity += accel * dt;
    this->setPosition(this->getPosition() + Vec2(0, this->velocity * dt));
}

void Character::onEnter() {
    Node::onEnter();
    this->scheduleUpdate();
}

void Character::jump() {
    this->velocity = 300;
    //this->setPosition(this->getPosition() + Vec2(0, this->velocity));
}