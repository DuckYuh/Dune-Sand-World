#include "Bullet.h"

USING_NS_CC;

Bullet* Bullet::createBullet(const Vec2& pos, bool facingRight)
{
    Bullet* bullet = new(std::nothrow) Bullet();
    if (bullet && bullet->initWithFile("images/Items/bullet.png"))
    {
        bullet->autorelease();
        bullet->setPosition(pos);
        bullet->startPos = pos;

        bullet->direction = facingRight ? Vec2(1, 0) : Vec2(-1, 0);
        bullet->setFlippedX(!facingRight);

        bullet->scheduleUpdate();
        return bullet;
    }
    CC_SAFE_DELETE(bullet);
    return nullptr;
}

void Bullet::update(float dt)
{
    this->setPosition(this->getPosition() + direction * speed * dt);

    float distance = this->getPosition().distance(startPos);
    if (distance >= maxDistance)
    {
        this->removeFromParent();
    }
}