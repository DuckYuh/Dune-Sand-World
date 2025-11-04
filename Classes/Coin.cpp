#include "Coin.h"
#include "GameManager.h"
#include "AudioManager.h"

USING_NS_CC;

Coin* Coin::create(const cocos2d::Vec2& pos)
{
	auto coin = new Coin();
    if (coin && coin->initWithFile("images/Items/Coins.png", Rect(0, 0, 32, 32)))
    {
        coin->autorelease();
        coin->scheduleUpdate();
        coin->initAnimations();
        coin->setPosition(pos);
        coin->runAction(MoveBy::create(0.2f, Vec2(0, -5)));
        coin->runAction(RepeatForever::create(Animate::create(AnimationCache::getInstance()->getAnimation("flip"))));
        return coin;
    }
    CC_SAFE_DELETE(coin);
    return nullptr;
}

void Coin::onPicked()
{
	GameManager::getInstance()->addCoin();

    AudioManager::getInstance()->playSFX("sounds/collect-coins.mp3");

    this->removeFromParent();
}

void Coin::initAnimations()
{
    Vector<SpriteFrame*> frames;

    auto exploseTex = Director::getInstance()->getTextureCache()->addImage("images/Items/Coins.png");
    for (int i = 0; i < 6; i++)
    {
        auto frame = SpriteFrame::createWithTexture(exploseTex, Rect(i * 32, 0, 32, 32));
        frames.pushBack(frame);
    }
    auto idleAnim = Animation::createWithSpriteFrames(frames, 0.2f);
    AnimationCache::getInstance()->addAnimation(idleAnim, "flip");
}