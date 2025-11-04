#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "cocos2d.h"

class Player;

class Enemy : public cocos2d::Sprite
{
private:
	bool movingRight = true;
	bool isGround = false;
	bool isDead = false;
	float speed = 50.0f;

	float patrolRange;
	float startX;
	std::string currentState = "idle";

	Player* player = nullptr;

	float shootCooldown = 0.0f;
public:
	static Enemy* createEnemy(const cocos2d::Vec2& startPos, float patrolRange);

	void initAnimations();

	void update(float dt) override;

	void changeState(const std::string& stateName);
	void patrol(); 
	void shootAtPlayer(Player* player);
	void setPlayer(Player* p) { player = p; }
	void onGround();
	void dead();
	bool getStatus() { return isDead; };
};

#endif // __ENEMY_H__