#ifndef __MELANGE_H__
#define __MELANGE_H__

#include "cocos2d.h"

class Player;

class Melange : public cocos2d::Sprite
{
private:

public:
    static Melange* create(const cocos2d::Vec2& pos);
    void onPicked(Player* player);
};

#endif // __MELANGE_H__