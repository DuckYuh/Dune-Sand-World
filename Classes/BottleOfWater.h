#ifndef __BOTTLE_OF_WATER_H__
#define __BOTTLE_OF_WATER_H__

#include "cocos2d.h"

class Player;

class BottleOfWater : public cocos2d::Sprite
{
private:
    
public:
    static BottleOfWater* create(const cocos2d::Vec2& pos);
    void onPicked(Player* player);
};

#endif // __BOTTLE_OF_WATER_H__