#ifndef __GAME_MANAGER_H__
#define __GAME_MANAGER_H__

#include "cocos2d.h"

class GameManager
{
private:
	int playerHealth = 3;
	int gameStage = 1;
	int coins = 0;
	float elapsedTime = 0.0f;
	static GameManager* instance;
public:
    static GameManager* getInstance();

	float getElapsedTime() const { return elapsedTime; }
	void setElapsedTime(float time) { elapsedTime = time; }
	void addElapsedTime(float delta) { elapsedTime += delta; }

	int getPlayerHealth() const { return playerHealth; };
	int getGameStage() const { return gameStage; };
	int getCoins() const { return coins; };

	void resetGame();
	void nextStage();
	void addCoin();
	void updatePlayerHealth(bool add);
	void fullHeal();
};

#endif //__GAME_MANAGER_H__