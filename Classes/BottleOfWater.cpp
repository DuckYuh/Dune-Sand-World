#include "BottleOfWater.h"
#include "Player.h"
#include "GameManager.h"
#include "AudioManager.h"

USING_NS_CC;

BottleOfWater* BottleOfWater::create(const Vec2& pos)
{
    auto item = new BottleOfWater();
    if (item && item->initWithFile("images/Items/BottleOfWater.png"))
    {
        item->autorelease();
        item->setPosition(pos);
        item->runAction(MoveBy::create(0.2f, Vec2(0, -5)));
        return item;
    }
    CC_SAFE_DELETE(item);
    return nullptr;
}

void BottleOfWater::onPicked(Player* player)
{
    GameManager::getInstance()->updatePlayerHealth(true);

    AudioManager::getInstance()->playSFX("sounds/drinking-water.mp3");

    this->removeFromParent();
}