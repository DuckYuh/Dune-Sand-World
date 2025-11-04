#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"

class GameScene : public cocos2d::Scene
{
private:
    std::vector<cocos2d::Rect> groundTiles;
    std::vector<cocos2d::Rect> wallTiles;
    std::vector<cocos2d::Rect> rearWallTiles;

    bool _isTransitioning = false;
    bool losing = false;

    std::vector<cocos2d::EventKeyboard::KeyCode> cheatSequence;
    std::vector<cocos2d::EventKeyboard::KeyCode> cheatInput;
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    void update(float delta) override;

    void drawTestMap();
	void drawTilemap();
    void drawBG();  
    void showUI();
    void pauseGame();
    void showEndGameBoard();

    void menuResumeCallback(Ref* pSender);
    void menuMainCallback(Ref* pSender);
    void menuCloseCallback(Ref* pSender);
    void createChunk(int idx, int pos);
	void nextGameScene();
    void toBossScene();

    CREATE_FUNC(GameScene);
};

#endif // __GAME_SCENE_H__