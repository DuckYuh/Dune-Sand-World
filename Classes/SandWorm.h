#ifndef __SANDWORM_H__
#define __SANDWORM_H__

#include "cocos2d.h"

class SandWorm : public cocos2d::Sprite
{
private:
	
public:
	static SandWorm* createSandWorm(const cocos2d::Vec2& pos);

	void initAnimations();
};

#endif // __SANDWORM_H__