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

std::vector<Rect> Obstacle::getRect() {
    std::vector<Rect> rect;
    float width = this->getChildByName("pipe_top")->getContentSize().width;
    float height = this->getChildByName("pipe_top")->getContentSize().height;
    
    rect.push_back(Rect(this->getPositionX(), this->getPositionY() + 50, width, height));
    rect.push_back(Rect(this->getPositionX(), this->getPositionY() - 50 - height, width, height));
    
    return rect;
}