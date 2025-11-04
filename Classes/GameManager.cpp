#include "GameManager.h"

USING_NS_CC;

GameManager* GameManager::instance = nullptr;

GameManager* GameManager::getInstance()
{
    if (!instance)
        instance = new GameManager();
    return instance;
}

void GameManager::resetGame()
{
	gameStage = 1;
}

void GameManager::nextStage()
{
	gameStage += 1;
}

void GameManager::addCoin()
{
	coins += 1;
}

void GameManager::updatePlayerHealth(bool add)
{
    if (add)
    {
        if (playerHealth < 3)    playerHealth += 1;
    }
    else
    {
        if (playerHealth > 0)    playerHealth -= 1;
    }
}

void GameManager::fullHeal()
{
    playerHealth = 3;
}