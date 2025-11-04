#ifndef __STONE_BURNER_H__
#define __STONE_BURNER_H__

#include "cocos2d.h"

class StoneBurner : public cocos2d::Sprite
{
private:

public:
    static StoneBurner* create(const cocos2d::Vec2& pos);

    void initAnimations();

	void beHitted();
};

#endif // __STONE_BURNER_H__