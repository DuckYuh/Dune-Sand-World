#include "StoneBurner.h"
#include "Melange.h"

USING_NS_CC;

StoneBurner* StoneBurner::create(const cocos2d::Vec2& pos)
{
    auto item = new StoneBurner();
    if (item && item->initWithFile("images/Items/StoneBurner.png", Rect(0, 0, 32, 32)))
    {
        item->autorelease();
        item->scheduleUpdate();
		item->initAnimations();
        item->setPosition(pos);
        return item;
    }
    CC_SAFE_DELETE(item);
    return nullptr;
}

void StoneBurner::initAnimations()
{
    Vector<SpriteFrame*> frames;

    auto exploseTex = Director::getInstance()->getTextureCache()->addImage("images/Items/StoneBurner.png");
    for (int i = 0; i < 7; i++)
    {
        auto frame = SpriteFrame::createWithTexture(exploseTex, Rect(i * 32, 0, 32, 32));
        frames.pushBack(frame);
    }
    auto idleAnim = Animation::createWithSpriteFrames(frames, 0.2f);
    AnimationCache::getInstance()->addAnimation(idleAnim, "explose");
}

void StoneBurner::beHitted()
{
    auto anim = AnimationCache::getInstance()->getAnimation("explose");
    if (!anim) return;

    auto animate = Animate::create(anim);

    auto spawnMelangeAndRemove = CallFunc::create([this]() {
        auto melange = Melange::create(this->getPosition());
        this->getParent()->addChild(melange);
        this->removeFromParentAndCleanup(true);
        });
    auto seq = Sequence::create(animate, spawnMelangeAndRemove, nullptr);
    this->runAction(seq);
}