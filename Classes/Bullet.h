#ifndef __BULLET_H__
#define __BULLET_H__

#include "cocos2d.h"

class Bullet : public cocos2d::Sprite
{
private:
    cocos2d::Vec2 direction;
    float speed = 400.0f;
    cocos2d::Vec2 startPos;
    float maxDistance = 200.0f;
public:
    static Bullet* createBullet(const cocos2d::Vec2& pos, bool facingRight);

    void update(float dt) override;
};

#endif // __BULLET_H__