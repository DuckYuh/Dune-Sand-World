#include "Melange.h"
#include "Player.h"
#include "AudioManager.h"

USING_NS_CC;

Melange* Melange::create(const cocos2d::Vec2& pos)
{
    auto item = new Melange();
    if (item && item->initWithFile("images/Items/Melange.png"))
    {
        item->autorelease();
        item->setPosition(pos);
        item->runAction(MoveBy::create(0.2f, Vec2(0, -5)));
        return item;
    }
    CC_SAFE_DELETE(item);
    return nullptr;
}

void Melange::onPicked(Player* player)
{
    player->enableMelangeBuff();

    Director::getInstance()->getScheduler()->unschedule("MelangeBuffTimer", player);

    Director::getInstance()->getScheduler()->schedule(
        [player](float) {
            if (player && player->getParent())  
                player->disableMelangeBuff();
        },
        player,
        0.0f, 
        0,     
        30.0f,   
        false,     
        "MelangeBuffTimer"
    );

    AudioManager::getInstance()->playSFX("sounds/boost.mp3");

    this->removeFromParent();
}