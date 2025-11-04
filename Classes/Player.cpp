#include "Player.h"
#include "Bullet.h"
#include "GameManager.h"
#include "AudioManager.h"

USING_NS_CC;

Player* Player::createPlayer()
{
    Player* player = new(std::nothrow) Player();

    if (player && player->initWithFile("images/sprites/Protagonist/idle.png", Rect(0, 0, 32, 32)))
    {
        player->autorelease();
        player->scheduleUpdate();
        player->initAnimations();
        return player;
    }

    CC_SAFE_DELETE(player);
    return nullptr;
}

void Player::initAnimations()
{
    Vector<SpriteFrame*> frames;

    auto idleTex = Director::getInstance()->getTextureCache()->addImage("images/sprites/Protagonist/idle.png");
    for (int i = 0; i < 5; i++) 
    {
        auto frame = SpriteFrame::createWithTexture(idleTex, Rect(i * 32, 0, 32, 32));
        frames.pushBack(frame);
    }
    auto idleAnim = Animation::createWithSpriteFrames(frames, 0.2f);
    AnimationCache::getInstance()->addAnimation(idleAnim, "idle");
    frames.clear();

    auto runTex = Director::getInstance()->getTextureCache()->addImage("images/sprites/Protagonist/run.png");
    for (int i = 0; i < 6; i++) 
    {
        auto frame = SpriteFrame::createWithTexture(runTex, Rect(i * 32, 0, 32, 32));
        frames.pushBack(frame);
    }
    auto runAnim = Animation::createWithSpriteFrames(frames, 0.15f);
    AnimationCache::getInstance()->addAnimation(runAnim, "run");
    frames.clear();

    auto jumpTex = Director::getInstance()->getTextureCache()->addImage("images/sprites/Protagonist/jump.png");
    for (int i = 0; i < 4; i++) 
    {
        auto frame = SpriteFrame::createWithTexture(jumpTex, Rect(i * 32, 0, 32, 32));
        frames.pushBack(frame);
    }
    auto jumpAnim = Animation::createWithSpriteFrames(frames, 0.2f);
    AnimationCache::getInstance()->addAnimation(jumpAnim, "jump");
    frames.clear();

    auto shootTex = Director::getInstance()->getTextureCache()->addImage("images/sprites/Protagonist/shoot.png");
    for (int i = 0; i < 4; i++) 
    {
        auto frame = SpriteFrame::createWithTexture(shootTex, Rect(i * 32, 0, 32, 32));
        frames.pushBack(frame);
    }
    auto shootAnim = Animation::createWithSpriteFrames(frames, 0.1f);
    AnimationCache::getInstance()->addAnimation(shootAnim, "shoot");
}

void Player::changeState(const std::string& state)
{
    if (currentState == state) return;

    currentState = state;

    this->stopAllActions();

    auto anim = AnimationCache::getInstance()->getAnimation(state);
    if (anim) 
    {
        auto animate = Animate::create(anim);
        if (state == "idle" || state == "run")
            this->runAction(RepeatForever::create(animate));
        else if (state == "jump")
            this->runAction(animate);
    }
}

void Player::update(float dt)
{
    if (isMovingLeft) 
    {
        this->setFlippedX(true);
        this->setPositionX(this->getPositionX() - speed * dt);
        if (!isJumping) changeState("run");
    }
    else if (isMovingRight) 
    {
        this->setFlippedX(false);
        this->setPositionX(this->getPositionX() + speed * dt);
        if (!isJumping) changeState("run");
    }
    else if (!isShooting)
    {
        if (!isJumping) changeState("idle");
    }

    if (isJumping) {
        jumpVelocity += gravity * dt;
        setPositionY(getPositionY() + jumpVelocity * dt);
    }
}

void Player::moveLeft()
{
    isShooting = false;
    isMovingLeft = true;
    isMovingRight = false;
}

void Player::moveRight()
{
    isShooting = false;
    isMovingRight = true;
    isMovingLeft = false;
}

void Player::stop()
{
    isMovingLeft = false;
    isMovingRight = false;
}

void Player::jump()
{
    if (isJumping == true)  return;
    isJumping = true;
    jumpVelocity = baseJumpVelocity;
    changeState("jump");
}

void Player::onGround()
{
    isJumping = false;
    jumpVelocity = 0;
}

void Player::fall()
{
    isJumping = true;
}

void Player::shoot()
{
    if (isShooting) return;

    isShooting = true;
    isMovingRight = false;
    isMovingLeft = false;

    auto anim = AnimationCache::getInstance()->getAnimation("shoot");
    if (anim)
    {
        auto animate = Animate::create(anim);
        this->runAction(animate);
    }

    auto spawnBullet = Sequence::create(
        DelayTime::create(0.1f),
        CallFunc::create([this]() {
            auto parent = this->getParent();
            if (parent)
            {
                bool facingRight = !this->isFlippedX();
                Vec2 bulletPos = this->getPosition() + Vec2(facingRight ? 25 : -25, 5);
                auto bullet = Bullet::createBullet(bulletPos, facingRight);
                if (bullet) parent->addChild(bullet, 5);
            }
            }),
        nullptr
    );
    this->runAction(spawnBullet);

    AudioManager::getInstance()->playSFX("sounds/shooting.mp3");

    float animDuration = anim ? anim->getDuration() : 0.4f;
    auto reset = Sequence::create(
        DelayTime::create(animDuration),
        CallFunc::create([this]() {
            isShooting = false;
            if (!isMovingLeft && !isMovingRight && !isJumping)
                changeState("idle");
            }),
        nullptr
    );
    this->runAction(reset);
}

void Player::resetPlayer()
{
    GameManager::getInstance()->updatePlayerHealth(false);
    setPosition(Vec2(200, 300));
}

void Player::enableMelangeBuff()
{
    GameManager::getInstance()->fullHeal();
    speed = 150;
    baseJumpVelocity = 450;
}

void Player::disableMelangeBuff()
{
    speed = 100;
    baseJumpVelocity = 320;
}