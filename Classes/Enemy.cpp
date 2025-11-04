#include "Enemy.h"
#include "Bullet.h"
#include "Player.h"
#include "AudioManager.h"

USING_NS_CC;

Enemy* Enemy::createEnemy(const Vec2& startPos, float patrolRange)
{
    Enemy* enemy = new(std::nothrow) Enemy();
    if (enemy && enemy->initWithFile("images/sprites/Harkonnen/idle.png", Rect(0, 0, 32, 32)))
    {
        enemy->autorelease();
        enemy->scheduleUpdate();
        enemy->initAnimations();
        enemy->setPosition(startPos);
        enemy->startX = startPos.x;
        enemy->patrolRange = patrolRange;
        enemy->changeState("e_idle");
        return enemy;
    }
    CC_SAFE_DELETE(enemy);
    return nullptr;
}

void Enemy::initAnimations()
{
    Vector<SpriteFrame*> frames;

    auto idleTex = Director::getInstance()->getTextureCache()->addImage("images/sprites/Harkonnen/idle.png");
    for (int i = 0; i < 2; i++) 
    {
        auto frame = SpriteFrame::createWithTexture(idleTex, Rect(i * 32, 0, 32, 32));
        frames.pushBack(frame);
    }
    auto idleAnim = Animation::createWithSpriteFrames(frames, 0.2f);
    AnimationCache::getInstance()->addAnimation(idleAnim, "e_idle");
    frames.clear();

    auto runTex = Director::getInstance()->getTextureCache()->addImage("images/sprites/Harkonnen/run.png");
    for (int i = 0; i < 2; i++) 
    {
        auto frame = SpriteFrame::createWithTexture(runTex, Rect(i * 32, 0, 32, 32));
        frames.pushBack(frame);
    }
    auto runAnim = Animation::createWithSpriteFrames(frames, 0.15f);
    AnimationCache::getInstance()->addAnimation(runAnim, "e_run");
    frames.clear();

    auto shootTex = Director::getInstance()->getTextureCache()->addImage("images/sprites/Harkonnen/shoot.png");
    for (int i = 0; i < 4; i++) 
    {
        auto frame = SpriteFrame::createWithTexture(shootTex, Rect(i * 32, 0, 32, 32));
        frames.pushBack(frame);
    }
    auto shootAnim = Animation::createWithSpriteFrames(frames, 0.1f);
    AnimationCache::getInstance()->addAnimation(shootAnim, "e_shoot");
}

void Enemy::changeState(const std::string& stateName)
{
    if (currentState == stateName) return; 
    currentState = stateName;

    auto anim = AnimationCache::getInstance()->getAnimation(stateName);
    if (anim)
    {
        this->stopActionByTag(100);
        auto action = RepeatForever::create(Animate::create(anim));
        action->setTag(100);
        this->runAction(action);
    }
}

void Enemy::update(float dt)
{
    if (!player) return;

    //isGround = false;

    if (isDead) return;

    shootCooldown -= dt;
    float distanceToPlayer = this->getPosition().distance(player->getPosition());

    if (!isGround)
    {
        setPositionY(getPositionY() - 500 * dt);
    }

    if (distanceToPlayer < 150.0f && player->getPositionY() - getPositionY() <= 32)
    {
        this->setFlippedX(player->getPositionX() < this->getPositionX());

        if (shootCooldown <= 0.0f)
        {
            changeState("e_shoot"); 
            shootAtPlayer(player);
            shootCooldown = 1.0f;
        }
        else
        {
            changeState("e_idle");
        }
    }
    else
    {
        patrol();
    }
}

void Enemy::patrol()
{
    float x = this->getPositionX();
    changeState("e_run"); 

    if (movingRight)
    {
        x += speed * Director::getInstance()->getDeltaTime();
        if (x > startX + patrolRange)
        {
            movingRight = false;
            this->setFlippedX(true);
        }
    }
    else
    {
        x -= speed * Director::getInstance()->getDeltaTime();
        if (x < startX - patrolRange)
        {
            movingRight = true;
            this->setFlippedX(false);
        }
    }
    this->setPositionX(x);
}

void Enemy::shootAtPlayer(Player* player)
{
    auto parent = this->getParent();
    if (!parent) return;

    bool facingRight = !this->isFlippedX();
    Vec2 bulletPos = this->getPosition() + Vec2(facingRight ? 25 : -25, 5);
    auto bullet = Bullet::createBullet(bulletPos, facingRight);
    if (bullet) parent->addChild(bullet, 5);

    AudioManager::getInstance()->playSFX("sounds/shooting.mp3");

}

void Enemy::onGround()
{
    isGround = true;
}

void Enemy::dead()
{
	isDead = true;
}