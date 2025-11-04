#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "cocos2d.h"

class Player : public cocos2d::Sprite
{
private:
    float speed = 100.0f;
    float jumpVelocity = 0;
    float gravity = -500;
    bool isJumping = false;
    bool isMovingLeft = false;
    bool isMovingRight = false;
    bool isShooting = false;
    float baseJumpVelocity = 320.0f; 
    std::string currentState = "idle";
public:
    static Player* createPlayer();

    void initAnimations();
    void changeState(const std::string& state);

    void update(float dt) override;

    void moveLeft();
    void moveRight();
    void stop();
    void jump();
    void shoot();
    void onGround();
	void fall();
    void resetPlayer();
    void enableMelangeBuff();
    void disableMelangeBuff();
};

#endif // __PLAYER_H__