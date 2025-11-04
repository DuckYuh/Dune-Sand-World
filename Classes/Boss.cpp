#include "Boss.h"
#include "Player.h"
#include "SandWorm.h"
#include "GameManager.h"
#include "BossScene.h"

USING_NS_CC;

Boss* Boss::createBoss(const cocos2d::Vec2& startPos)
{
	Boss* boss = new(std::nothrow) Boss();
	if (boss && boss->initWithFile("images/sprites/Leto/idle.png", Rect(0, 0, 32, 32)))
	{
		boss->autorelease();
		boss->scheduleUpdate();
		boss->initAnimations();
		boss->setPosition(startPos);
		boss->changeState("b_idle");
		return boss;
	}
	CC_SAFE_DELETE(boss);
	return nullptr;
}

void Boss::initAnimations()
{
	Vector<SpriteFrame*> frames;

	auto idleTex = Director::getInstance()->getTextureCache()->addImage("images/sprites/Leto/idle.png");
	for (int i = 0; i < 3; i++) 
	{
		auto frame = SpriteFrame::createWithTexture(idleTex, Rect(i * 32, 0, 32, 32));
		frames.pushBack(frame);
	}
	auto idleAnim = Animation::createWithSpriteFrames(frames, 0.2f);
	AnimationCache::getInstance()->addAnimation(idleAnim, "b_idle");
	frames.clear();

	auto runTex = Director::getInstance()->getTextureCache()->addImage("images/sprites/Leto/run.png");
	for (int i = 0; i < 2; i++) 
	{
		auto frame = SpriteFrame::createWithTexture(runTex, Rect(i * 32, 0, 32, 32));
		frames.pushBack(frame);
	}
	auto runAnim = Animation::createWithSpriteFrames(frames, 0.15f);
	AnimationCache::getInstance()->addAnimation(runAnim, "b_run");
	frames.clear();

	auto attackTex = Director::getInstance()->getTextureCache()->addImage("images/sprites/Leto/attack.png");
	for (int i = 0; i < 4; i++) 
	{
		auto frame = SpriteFrame::createWithTexture(attackTex, Rect(i * 32, 0, 32, 32));
		frames.pushBack(frame);
	}
	auto attackAnim = Animation::createWithSpriteFrames(frames, 0.1f);
	AnimationCache::getInstance()->addAnimation(attackAnim, "b_attack");
}

void Boss::update(float dt)
{
	if (!player) return;

	if (!isGround)
	{
		setPositionY(getPositionY() - 500 * dt);
	}

	if (health <= 5 && !enraged)	enragedPhase();

	sandwormTimer += dt;
	if (sandwormTimer >= sandwormCooldown)
	{
		spawnSandWorm();
		sandwormTimer = 0.0f; 
	}

	float distance = std::abs(player->getPositionX() - getPositionX());
	float direction = (player->getPositionX() > getPositionX()) ? 1.0f : -1.0f;

	if (distance <= 250.0f)
	{
		setPositionX(getPositionX() - direction * speed * dt);
		changeState("b_run");
	}
	else
	{
		setPositionX(getPositionX() + direction * speed * dt);
		changeState("b_run");
	}

	setFlippedX(direction < 0);
}

void Boss::changeState(const std::string& stateName)
{
	if (currentState == stateName) return;
	currentState = stateName;

	auto anim = AnimationCache::getInstance()->getAnimation(stateName);
	if (anim)
	{
		this->stopActionByTag(200);
		auto action = RepeatForever::create(Animate::create(anim));
		action->setTag(200);
		this->runAction(action);
	}
}

void Boss::spawnSandWorm()
{
	if (!player) return;

	auto sandworm = SandWorm::createSandWorm(player->getPosition());
	if (sandworm)
	{
		this->getParent()->addChild(sandworm);

		sandworm->runAction(Sequence::create(
			DelayTime::create(0.7f),
			CallFunc::create([this, sandworm]() {
				if (player && sandworm->getBoundingBox().intersectsRect(player->getBoundingBox()))
				{
					GameManager::getInstance()->updatePlayerHealth(false);
					static_cast<BossScene*>(this->getParent())->showUI();
				}
				}),
			DelayTime::create(0.1f),
			RemoveSelf::create(), 
			nullptr
		));
	}
}

void Boss::enragedPhase()
{
	enraged = true;
	this->runAction(TintTo::create(0.5f, 255, 50, 50));
	sandwormCooldown = 1.0f;
	speed = 80.0f;
}

void Boss::onGround()
{
	isGround = true;
}

void Boss::beHitted()
{
	health -= 1;
}

void Boss::kill()
{
	health = 0;
}
