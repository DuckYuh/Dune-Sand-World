#ifndef __BOSS_SCENE_H__
#define __BOSS_SCENE_H__

#include "cocos2d.h"

class BossScene : public cocos2d::Scene
{
private:
    std::vector<cocos2d::Rect> groundTiles;
    std::vector<cocos2d::Rect> wallTiles;

    std::vector<cocos2d::EventKeyboard::KeyCode> cheatSequence;
    std::vector<cocos2d::EventKeyboard::KeyCode> cheatInput;
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    void update(float delta) override;

    void drawBG();
	void drawTileMap();
    void showUI();
    void pauseGame();
    void showEndGameBoard();
    void winBroad();

    void menuResumeCallback(Ref* pSender);
    void menuMainCallback(Ref* pSender);
    void menuCloseCallback(Ref* pSender);

    CREATE_FUNC(BossScene);
};

#endif // __BOSS_SCENE_H__