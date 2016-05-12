#ifndef __MAIN_SCENE_H__
#define __MAIN_SCENE_H__

#include "cocos2d.h"
#include "Character.hpp"
#include "Obstacle.hpp"

class MainScene : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // implement the "static create()" method manually
    CREATE_FUNC(MainScene);
    
    void onEnter() override;
    void update(float dt) override;
    void createObstacle(float dt);
    
private:
    Character* character;
    cocos2d::Vector<Obstacle*> obstacles;
    cocos2d::Node* backgournd;
    
    void setupTouchHandling();
};

#endif // __MAIN_SCENE_H__