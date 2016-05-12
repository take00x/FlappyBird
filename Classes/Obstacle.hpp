//
//  Obstacle.hpp
//  FlappyBird
//
//  Created by Takehiro Yoshihama on 2016/05/12.
//
//

#ifndef Obstacle_hpp
#define Obstacle_hpp

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"

class Obstacle : public cocos2d::Node
{
public:
    CREATE_FUNC(Obstacle);
    bool init() override;
    void moveLeft(float distance);
    
protected:
    cocostudio::timeline::ActionTimeline* timeline;
};

#endif /* Obstacle_hpp */