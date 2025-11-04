#ifndef __COIN_H__
#define __COIN_H__

#include "cocos2d.h"

class Coin : public cocos2d::Sprite
{
private:

public:
	static Coin* create(const cocos2d::Vec2& pos);
	void onPicked();
	void initAnimations();
};

#endif // __COIN_H__