#ifndef __BOSS_H__
#define __BOSS_H__

#include "cocos2d.h"

class Player;

class Boss : public cocos2d::Sprite
{
private:
	float speed = 50.0f;
	bool isGround = false;
	bool enraged = false;
	bool isAttacking = false;
	bool isDead = false;
	int health = 15;

	Player* player = nullptr;

	std::string currentState = "idle";

	float sandwormCooldown = 2.0f; 
	float sandwormTimer = 0.0f;
public:
	static Boss* createBoss(const cocos2d::Vec2& startPos);

	void initAnimations();

	void update(float dt) override;

	void changeState(const std::string& stateName);
	void spawnSandWorm();
	void setPlayer(Player* p) { player = p; }
	void onGround();
	void beHitted();
	void kill();
	int getHealth() const { return health; };
	void enragedPhase();
};

#endif // __BOSS_H__