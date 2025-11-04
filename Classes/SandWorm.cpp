#include "SandWorm.h"

USING_NS_CC;

SandWorm* SandWorm::createSandWorm(const cocos2d::Vec2& pos)
{
	SandWorm* worm = new(std::nothrow) SandWorm();
	if (worm && worm->initWithFile("images/sprites/SandWorm/attack.png", Rect(0, 0, 32, 32)))
	{
		worm->autorelease();
		worm->initAnimations();
		worm->setPosition(pos);
		worm->scheduleUpdate();
		worm->runAction(RepeatForever::create(Animate::create(AnimationCache::getInstance()->getAnimation("s_attack"))));
		return worm;
	}
	CC_SAFE_DELETE(worm);
	return nullptr;
}

void SandWorm::initAnimations()
{
	Vector<SpriteFrame*> frames;

	auto attackTex = Director::getInstance()->getTextureCache()->addImage("images/sprites/SandWorm/attack.png");
	for (int i = 0; i < 8; i++)
	{
		auto frame = SpriteFrame::createWithTexture(attackTex, Rect(i * 32, 0, 32, 32));
		frames.pushBack(frame);
	}
	auto attackAnim = Animation::createWithSpriteFrames(frames, 0.1f);
	AnimationCache::getInstance()->addAnimation(attackAnim, "s_attack");
}