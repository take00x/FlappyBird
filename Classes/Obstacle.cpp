//
//  Obstacle.cpp
//  FlappyBird
//
//  Created by Takehiro Yoshihama on 2016/05/12.
//
//

#include "Obstacle.hpp"

using namespace cocos2d;

bool Obstacle::init()
{
    if (! Node::init())
    {
        return false;
    }
    
    // キャラクターのアニメーションのタイムラインを読み込む
    this->timeline = CSLoader::createTimeline("Obstacle.csb");
    
    // キャラクターのアニメーションのタイムラインを確保し、メモリ領域が解放されないようにする
    this->timeline->retain();
    
    return true;
}

void Obstacle::moveLeft(float distance) {
    this->setPosition(this->getPosition() + Vec2(-distance, 0));
}