#include "GameScene.h"
#include "BossScene.h"
#include "MainMenuScene.h"
#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"
#include "BottleOfWater.h"
#include "Melange.h"
#include "StoneBurner.h"
#include "Coin.h"
#include "GameManager.h"
#include "AudioManager.h"

USING_NS_CC;

Scene* GameScene::createScene()
{
    return GameScene::create();
}

bool GameScene::init()
{
    if (!Scene::init())
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    _isTransitioning = false;

    drawBG();
        
    auto player = Player::createPlayer();
    player->setPosition(origin.x + 200, origin.y + 300);
    this->addChild(player, 1, "player");
    player->stop();

    drawTilemap();
	

    if (random(0,1))
    {
        if (random(0,1))
        {
            auto stoneburner = StoneBurner::create(Vec2(1280, 144));
            this->addChild(stoneburner, 0, "StoneBurner");
        }
		else
        {
            auto stoneburner = StoneBurner::create(Vec2(2560, 144));
            this->addChild(stoneburner, 0, "StoneBurner");
        }
    }

    AudioManager::getInstance()->playSFX("sounds/game-start.mp3");

    auto uiLayer = Node::create();
    uiLayer->setName("UILayer");
    this->addChild(uiLayer, 1000);

    cheatSequence = {
        EventKeyboard::KeyCode::KEY_UP_ARROW,
        EventKeyboard::KeyCode::KEY_UP_ARROW,
        EventKeyboard::KeyCode::KEY_DOWN_ARROW,
        EventKeyboard::KeyCode::KEY_DOWN_ARROW,
        EventKeyboard::KeyCode::KEY_LEFT_ARROW,
        EventKeyboard::KeyCode::KEY_RIGHT_ARROW,
        EventKeyboard::KeyCode::KEY_LEFT_ARROW,
        EventKeyboard::KeyCode::KEY_RIGHT_ARROW,
        EventKeyboard::KeyCode::KEY_Z,
        EventKeyboard::KeyCode::KEY_X
    };
    cheatInput.clear();

    auto listener = EventListenerKeyboard::create();
    listener->onKeyPressed = [this, player, uiLayer](EventKeyboard::KeyCode keyCode, Event* event)
        {
            cheatInput.push_back(keyCode);

            if (cheatInput.size() > cheatSequence.size())   cheatInput.erase(cheatInput.begin());

            if (cheatInput == cheatSequence)
            {
                GameManager::getInstance()->resetGame();
                toBossScene();
                cheatInput.clear();
                return;
            }

            if (keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW)  player->moveLeft();
            if (keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW)  player->moveRight();
            if (keyCode == EventKeyboard::KeyCode::KEY_X)  player->jump();
            if (keyCode == EventKeyboard::KeyCode::KEY_Z)  player->shoot();
            /*if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE)  menuCloseCallback(this);*/
            if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE) 
            {
                if (uiLayer->getChildByName("EndGameBoard"))
                {
					menuMainCallback(this);
                }
                else if (!uiLayer->getChildByName("PauseMenu"))
                {
                    pauseGame();
                }
                else
                {
                    menuResumeCallback(this);
                }
            }
        };
    listener->onKeyReleased = [player](EventKeyboard::KeyCode keyCode, Event* event)
        {
            if (keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW ||
                keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW)
            {
                player->stop();
            }
        };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    auto camera = Follow::create(player, Rect(0, 0, 5100, visibleSize.height));
    this->runAction(camera);

    showUI();

    scheduleUpdate();

    return true;
}

void GameScene::drawBG()
{
    auto sky = Director::getInstance()->getTextureCache()->addImage("images/Backgrounds/Desert Background Desert Layer 5.png");
    int skyW = sky->getPixelsWide();

    for (int i = 0; i < 8; i++) 
    {
        auto spr = Sprite::createWithTexture(sky);
        spr->setAnchorPoint(Vec2::ZERO);
        spr->setScale(1.6f);

        float sprH = spr->getContentSize().height * spr->getScale();
        spr->setPosition(i * skyW * 1.6f, 720 - sprH);
        this->addChild(spr, -3);
    }

    auto sun = Sprite::create("images/Backgrounds/BG-sun.png");
    sun->setPosition(1000,500);
    this->addChild(sun, -3);

    auto cloud = Director::getInstance()->getTextureCache()->addImage("images/Backgrounds/Desert Background Desert Layer 4.png");
    int cloudW = cloud->getPixelsWide();

    for (int i = 0; i < 8; i++)
    {
        auto spr = Sprite::createWithTexture(cloud);
        spr->setAnchorPoint(Vec2::ZERO);
        float sprH = spr->getContentSize().height;
        spr->setPosition(i * cloudW * 1.6f, 720 - sprH);
        this->addChild(spr, -3);
    }

    auto bg = Director::getInstance()->getTextureCache()->addImage("images/Backgrounds/Desert Background Desert Layer 1.png");
    int bgW = bg->getPixelsWide();

    for (int i = 0; i < 8; i++)
    {
        auto spr = Sprite::createWithTexture(bg);
        spr->setAnchorPoint(Vec2::ZERO);
        spr->setScale(1.6f);
      
        spr->setPosition(i * bgW * 1.6f, 0);
        this->addChild(spr, -2);
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();

    auto stageLabel = Label::createWithTTF("Stage " + std::to_string(GameManager::getInstance()->getGameStage()), "fonts/Marker Felt.ttf", 24);
    stageLabel->setPosition(Vec2(50, visibleSize.height - 50));
    stageLabel->setTextColor(Color4B::WHITE);
    stageLabel->enableOutline(Color4B::BLACK, 1);
    this->addChild(stageLabel, -2);

    auto nextLabel = Label::createWithTTF("Next Stage", "fonts/GomePixel-DYJX1.otf", 24);
    nextLabel->setPosition(Vec2(visibleSize.width * 4 - 100, visibleSize.height / 2 - 100));
    nextLabel->setTextColor(Color4B::WHITE);
    nextLabel->enableOutline(Color4B::BLACK, 1);
    this->addChild(nextLabel, -2);
}

void GameScene::drawTestMap()
{
    auto gameSize = Director::getInstance()->getVisibleSize();

    auto texture = Director::getInstance()->getTextureCache()->addImage("images/Tileset/ground.png");

    int texWidth = texture->getPixelsWide();
    int texHeight = texture->getPixelsHigh();

    int tileSize = 32;

    std::vector<SpriteFrame*> tiles;
    for (int y = texHeight - tileSize; y >= 0; y -= tileSize) 
    {
        for (int x = 0; x < texWidth; x += tileSize) 
        {
            Rect rect(x, y, tileSize, tileSize);
            auto frame = SpriteFrame::createWithTexture(texture, rect);
            tiles.push_back(frame);
        }
    }

    int gameRow = gameSize.height / tileSize;
    int gameCol = (gameSize.width / tileSize) * 4;

    for (int col = 0; col < gameCol; col++)
    {
        int row = 0;
        for (row; row < 3; row++)
        {
            auto spr = Sprite::createWithSpriteFrame(tiles[9]);
            spr->setAnchorPoint(Vec2::ZERO);
            spr->setPosition(col * tileSize, tileSize * row);
            this->addChild(spr);
        }

        auto spr = Sprite::createWithSpriteFrame(tiles[13]);
        spr->setAnchorPoint(Vec2::ZERO);
        spr->setPosition(col * tileSize, tileSize * row);
        this->addChild(spr);

        groundTiles.push_back(spr->getBoundingBox());
    }
}

void GameScene::drawTilemap()
{
    for (int i = 0; i < 4; i++)
    {
        createChunk(random(1, 5), i);
	}
}

void GameScene::showUI()
{
    auto player = dynamic_cast<Player*>(this->getChildByName("player"));
    auto uiLayer = this->getChildByName("UILayer");
    if (!player || !uiLayer) return;

    auto visibleSize = Director::getInstance()->getVisibleSize();

    uiLayer->removeAllChildren();

    float y = visibleSize.height - 32;

    int health = GameManager::getInstance()->getPlayerHealth();
    for (int i = 0; i < 3; i++)
    {
        std::string texture = (i < health) ? "images/ui/HeartFull.png" : "images/ui/HeartEmpty.png";
        auto heart = Sprite::create(texture);
        heart->setPosition(50 + (i - 1) * 16, y);
        uiLayer->addChild(heart);
    }

    auto coin = Sprite::create("images/Items/Coins.png", Rect(0, 0, 32, 32));
    coin->setPosition(visibleSize.width - 100, y);
    uiLayer->addChild(coin);

    int coinCount = GameManager::getInstance()->getCoins();
    auto coinLabel = Label::createWithTTF(std::to_string(coinCount), "fonts/Marker Felt.ttf", 16);
    coinLabel->setPosition(visibleSize.width - 80, y);
    coinLabel->setTextColor(Color4B::WHITE);
    coinLabel->enableOutline(Color4B::BLACK, 1);
    uiLayer->addChild(coinLabel);
}

void GameScene::pauseGame()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    auto player = dynamic_cast<Player*>(this->getChildByName("player"));
    auto uiLayer = this->getChildByName("UILayer");
    if (!player || !uiLayer) return;

    auto popupBg = LayerColor::create(Color4B(0, 0, 0, 128));
    popupBg->setContentSize(visibleSize);
    popupBg->setName("PauseMenu");
    uiLayer->addChild(popupBg, 999); 

    auto panel = Sprite::create("images/ui/OptionBoard.png");
    panel->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 - 50);
    popupBg->addChild(panel);

    auto pauseLabel = Label::createWithTTF("Game Pause", "fonts/GomePixel-DYJX1.otf", 48);
    pauseLabel->setPosition(panel->getContentSize().width / 2, panel->getContentSize().height - 80);
    panel->addChild(pauseLabel);

    auto timeElapsed = GameManager::getInstance()->getElapsedTime();
    int minutes = static_cast<int>(timeElapsed) / 60;
    int seconds = static_cast<int>(timeElapsed) % 60;

    auto timerLabel = Label::createWithTTF("Time: " + std::to_string(minutes) + "m " + std::to_string(seconds) + "s",
        "fonts/Marker Felt.ttf", 24);
    timerLabel->setPosition(panel->getContentSize().width / 2, panel->getContentSize().height / 2 + 50);
    timerLabel->setTextColor(Color4B::WHITE);
    timerLabel->enableOutline(Color4B::BLACK, 1);
    panel->addChild(timerLabel);

    auto resumeItem = MenuItemImage::create("images/ui/Button.png",
        "images/ui/ButtonPressed.png",
        CC_CALLBACK_1(GameScene::menuResumeCallback, this));
    resumeItem->setPosition(Vec2(panel->getContentSize().width / 2 - 200, panel->getContentSize().height / 2 - 100));
    auto resumeLabel = Label::createWithTTF("Resume", "fonts/GomePixel-DYJX1.otf", 36);
    resumeLabel->setPosition(resumeItem->getContentSize() / 2);
    resumeLabel->setTextColor(Color4B::WHITE);
    resumeLabel->enableOutline(Color4B::BLACK, 2);
    resumeItem->addChild(resumeLabel);

    auto menuItem = MenuItemImage::create("images/ui/Button.png",
        "images/ui/ButtonPressed.png",
        CC_CALLBACK_1(GameScene::menuMainCallback, this));
    menuItem->setPosition(Vec2(panel->getContentSize().width / 2, panel->getContentSize().height / 2 - 100));
    auto menuLabel = Label::createWithTTF("Menu", "fonts/GomePixel-DYJX1.otf", 36);
    menuLabel->setPosition(menuItem->getContentSize() / 2);
    menuLabel->setTextColor(Color4B::WHITE);
    menuLabel->enableOutline(Color4B::BLACK, 2);
    menuItem->addChild(menuLabel);

    auto exitItem = MenuItemImage::create("images/ui/Button.png",
        "images/ui/ButtonPressed.png",
        CC_CALLBACK_1(GameScene::menuCloseCallback, this));
    exitItem->setPosition(Vec2(panel->getContentSize().width / 2 + 200, panel->getContentSize().height / 2 - 100));
    auto exitLabel = Label::createWithTTF("Exit", "fonts/GomePixel-DYJX1.otf", 36);
    exitLabel->setPosition(exitItem->getContentSize() / 2);
    exitLabel->setTextColor(Color4B::WHITE);
    exitLabel->enableOutline(Color4B::BLACK, 2);
    exitItem->addChild(exitLabel);

    auto menu = Menu::create(resumeItem, menuItem, exitItem, nullptr);
    menu->setPosition(Vec2::ZERO);
    panel->addChild(menu);

    Director::getInstance()->pause();
}

void GameScene::showEndGameBoard()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto uiLayer = this->getChildByName("UILayer");
    if (!uiLayer) return;

    if (uiLayer->getChildByName("EndGameBoard")) return;

    Director::getInstance()->pause();

    auto popupBg = LayerColor::create(Color4B(0, 0, 0, 192));
    popupBg->setContentSize(visibleSize);
    popupBg->setName("EndGameBoard");
    uiLayer->addChild(popupBg, 1000);

    auto panel = Sprite::create("images/ui/OptionBoard.png");
    panel->setPosition(visibleSize.width / 2, visibleSize.height / 2);
    popupBg->addChild(panel);

    auto gameOverLabel = Label::createWithTTF("Game Over", "fonts/GomePixel-DYJX1.otf", 48);
    gameOverLabel->setPosition(panel->getContentSize().width / 2, panel->getContentSize().height - 80);
    gameOverLabel->setTextColor(Color4B::WHITE);
    gameOverLabel->enableOutline(Color4B::BLACK, 2);
    panel->addChild(gameOverLabel);

    int coins = GameManager::getInstance()->getCoins();
    auto coinsLabel = Label::createWithTTF("Coins: " + std::to_string(coins), "fonts/Marker Felt.ttf", 36);
    coinsLabel->setPosition(panel->getContentSize().width / 2, panel->getContentSize().height / 2 + 20);
    coinsLabel->setTextColor(Color4B::WHITE);
    coinsLabel->enableOutline(Color4B::BLACK, 1);
    panel->addChild(coinsLabel);

    float elapsedTime = GameManager::getInstance()->getElapsedTime();
    int minutes = static_cast<int>(elapsedTime) / 60;
    int seconds = static_cast<int>(elapsedTime) % 60;
    auto timeLabel = Label::createWithTTF("Time: " + std::to_string(minutes) + "m " + std::to_string(seconds) + "s", "fonts/Marker Felt.ttf", 36);
    timeLabel->setPosition(panel->getContentSize().width / 2, panel->getContentSize().height / 2 - 20);
    timeLabel->setTextColor(Color4B::WHITE);
    timeLabel->enableOutline(Color4B::BLACK, 1);
    panel->addChild(timeLabel);

    auto escLabel = Label::createWithTTF("Press ESC to return to Main Menu", "fonts/Marker Felt.ttf", 24);
    escLabel->setPosition(panel->getContentSize().width / 2, 50);
    escLabel->setTextColor(Color4B::WHITE);
    escLabel->enableOutline(Color4B::BLACK, 1);
    panel->addChild(escLabel);
}

void GameScene::update(float delta)
{
    GameManager::getInstance()->addElapsedTime(delta);

    auto player = dynamic_cast<Player*>(this->getChildByName("player"));
    auto uiLayer = this->getChildByName("UILayer");
    auto visibleSize = Director::getInstance()->getVisibleSize();

    if (player && uiLayer)
    {
        float offsetX = player->getPositionX() - visibleSize.width / 2;
        offsetX = clampf(offsetX, 0, 5100 - visibleSize.width);

        uiLayer->setPositionX(offsetX);
    }

    if (player && GameManager::getInstance()->getPlayerHealth() <= 0)
    {
        showEndGameBoard();
        losing = true;
        return;
    }

    std::vector<Enemy*> enemies;
    for (auto node : this->getChildren())
    {
        auto enemy = dynamic_cast<Enemy*>(node);
        if (enemy) enemies.push_back(enemy);
    }

    if (player->getPositionX() <= 16)
    {
        player->setPositionX(16);
    }
    if (player->getPositionX() >= 5120)
    {
        player->setPositionX(5120);
    }
    if (!_isTransitioning && player->getPositionX() >= visibleSize.width * 4)
    {
        _isTransitioning = true;
        GameManager::getInstance()->nextStage();
        if (GameManager::getInstance()->getGameStage() >= 5)
        {
            GameManager::getInstance()->resetGame();
            toBossScene();
        }
		else nextGameScene();
    }

    if (player->getPositionY() <= 0)
    {
		player->resetPlayer();
    }

    bool onGround = false;

    for (auto& rect : groundTiles)
    {
        if (player->getBoundingBox().intersectsRect(rect))
        {
            player->setPositionY(rect.getMaxY() + 16);
            player->onGround();
            onGround = true;
            break;
        }
		for (auto enemy : enemies)
        {
            if (enemy && enemy->getBoundingBox().intersectsRect(rect))
            {
                enemy->setPositionY(rect.getMaxY() + 16);
                enemy->onGround();
            }
        }
    }
    for (auto& rect : wallTiles)
    {
        if (player->getBoundingBox().intersectsRect(rect))
        {
            Rect playerBox = player->getBoundingBox();

            if (playerBox.getMaxX() > rect.getMinX() && player->getPositionX() < rect.getMinX())
            {
                player->setPositionX(rect.getMinX() - playerBox.size.width / 2);
            }

            else if (playerBox.getMinX() < rect.getMaxX() && player->getPositionX() > rect.getMaxX())
            {
                player->setPositionX(rect.getMaxX() + playerBox.size.width / 2);
            }
        }
    }
    for (auto& rect : rearWallTiles)
    {
        if (player->getBoundingBox().intersectsRect(rect))
        {
            Rect playerBox = player->getBoundingBox();
            if (playerBox.getMinY() >= rect.getMaxY() - 3 && playerBox.getMinY() <= rect.getMaxY() + 3)
            {
                player->setPositionY(rect.getMaxY() + 16);
                player->onGround();
                onGround = true;
                break;
            }
            else
            {
                if (playerBox.getMaxX() > rect.getMinX() && player->getPositionX() < rect.getMinX())
                {
                    player->setPositionX(rect.getMinX() - playerBox.size.width / 2);
                }
                else if (playerBox.getMinX() < rect.getMaxX() && player->getPositionX() > rect.getMaxX())
                {
                    player->setPositionX(rect.getMaxX() + playerBox.size.width / 2);
                }
            }
        }
	}

    if (!onGround)
    {
        player->fall();
    }

    std::vector<Bullet*> bullets;
    for (auto node : this->getChildren())
    {
        auto bullet = dynamic_cast<Bullet*>(node);
        if (bullet) bullets.push_back(bullet);
    }

    std::vector<Enemy*> enemiesToRemove;

    auto stoneBurner = dynamic_cast<StoneBurner*>(this->getChildByName("StoneBurner"));

    for (auto bullet : bullets)
    {
        bool removed = false;
        for (auto& rect : wallTiles)
        {
            if (bullet->getBoundingBox().intersectsRect(rect))
            {
                bullet->removeFromParent();
                removed = true;
                break;
            }
        }
        if (removed) continue;

        if (player && bullet->getBoundingBox().intersectsRect(player->getBoundingBox()))
        {
            bullet->removeFromParent();
            if (!losing)    GameManager::getInstance()->updatePlayerHealth(false);
            showUI();
            continue;
        }

        if (stoneBurner && bullet->getBoundingBox().intersectsRect(stoneBurner->getBoundingBox()))
        {
            bullet->removeFromParent();
            stoneBurner->beHitted();
            break;
        }

        for (auto enemy : enemies)
        {
            if (enemy && !enemy->getStatus() && bullet->getBoundingBox().intersectsRect(enemy->getBoundingBox()))
            {
                bullet->removeFromParent();
				enemy->dead();
                auto roll = random(0, 9);
                if (roll < 3)
                {
                    auto bottle = BottleOfWater::create(enemy->getPosition());
                    this->addChild(bottle);
                }
                if (roll >= 3 && roll < 9)
                {
                    auto coin = Coin::create(enemy->getPosition());
                    this->addChild(coin);
                }
                enemiesToRemove.push_back(enemy); 
                break;
            }
        }
    }

    for (auto enemy : enemiesToRemove)
    {
        enemy->runAction(Sequence::create(
            FadeOut::create(0.5f),
            CallFunc::create([enemy]() {
                enemy->removeFromParent();
                }),
            nullptr
        ));
    }

    std::vector<BottleOfWater*> bottles;
    for (auto node : this->getChildren())
    {
        auto bottle = dynamic_cast<BottleOfWater*>(node);
        if (bottle) bottles.push_back(bottle);
    }

    for (auto bottle : bottles)
    {
        if (player->getBoundingBox().intersectsRect(bottle->getBoundingBox()))
        {
            bottle->onPicked(player);
            showUI();
        }
    }

    std::vector<Melange*> melanges;
    for (auto node : this->getChildren())
    {
        auto melange = dynamic_cast<Melange*>(node);
        if (melange) melanges.push_back(melange);
    }

    for (auto melange : melanges)
    {
        if (player->getBoundingBox().intersectsRect(melange->getBoundingBox()))
        {
            melange->onPicked(player);
            showUI();
        }
    }

    std::vector<Coin*> coins;
    for (auto node : this->getChildren())
    {
        auto coin = dynamic_cast<Coin*>(node);
        if (coin) coins.push_back(coin);
    }

    for (auto coin : coins)
    {
        if (player->getBoundingBox().intersectsRect(coin->getBoundingBox()))
        {
            coin->onPicked();
            showUI();
        }
    }
}

void GameScene::menuResumeCallback(Ref* pSender)
{
    auto uiLayer = this->getChildByName("UILayer");
    uiLayer->removeChildByName("PauseMenu");
    Director::getInstance()->resume();
}

void GameScene::menuMainCallback(Ref* pSender)
{
    Director::getInstance()->resume();
    GameManager::getInstance()->resetGame();
    GameManager::getInstance()->fullHeal();
    GameManager::getInstance()->setElapsedTime(0);
    auto menuScene = MainMenuScene::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(1.0f, menuScene));
}

void GameScene::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();
}

void GameScene::nextGameScene()
{
    auto gameScene = GameScene::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(1.0f, gameScene));
}

void GameScene::toBossScene()
{
    auto bossScene = BossScene::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(1.0f, bossScene));
}

void GameScene::createChunk(int idx, int pos)
{
    auto gameSize = Director::getInstance()->getVisibleSize();

    auto texture = Director::getInstance()->getTextureCache()->addImage("images/Tileset/grounds.png");

    auto player = dynamic_cast<Player*>(this->getChildByName("player"));

    int texWidth = texture->getPixelsWide();
    int texHeight = texture->getPixelsHigh();

    int tileSize = 32;

    std::vector<SpriteFrame*> tiles;
    for (int y = texHeight - tileSize; y >= 0; y -= tileSize) 
    {
        for (int x = 0; x < texWidth; x += tileSize) 
        {
            Rect rect(x, y, tileSize, tileSize);
            auto frame = SpriteFrame::createWithTexture(texture, rect);
            tiles.push_back(frame);
        }
    }

    int gameRow = gameSize.height / tileSize;
    int gameCol = gameSize.width / tileSize;

    switch (idx)
    {
        case 1:
        {
            for (int col = 0; col < gameCol; col++)
            {
                int row = 0;
                for (row; row < 3; row++)
                {
                    auto spr = Sprite::createWithSpriteFrame(tiles[15]);
                    spr->setAnchorPoint(Vec2::ZERO);
                    spr->setPosition(col * tileSize + gameSize.width * pos, tileSize * row);
                    this->addChild(spr);
                }

                auto spr = Sprite::createWithSpriteFrame(tiles[22]);
                spr->setAnchorPoint(Vec2::ZERO);
                spr->setPosition(col * tileSize + gameSize.width * pos, tileSize * row);
                this->addChild(spr);

                groundTiles.push_back(spr->getBoundingBox());
            }
            auto enemy = Enemy::createEnemy(Vec2(gameSize.width / 2 + gameSize.width * pos, 300), 100.0f);
            enemy->setPlayer(player);
            this->addChild(enemy, 1);
            break;
        }
        case 2:
        {
            for (int col = 0; col <= gameCol / 2 - 5; col++)
            {
                int row = 0;
                for (row; row < 3; row++)
                {
                    auto spr = Sprite::createWithSpriteFrame(tiles[15]);
                    spr->setAnchorPoint(Vec2::ZERO);
                    spr->setPosition(col * tileSize + gameSize.width * pos, tileSize * row);
                    this->addChild(spr);
                }

                auto spr = Sprite::createWithSpriteFrame(tiles[22]);
                spr->setAnchorPoint(Vec2::ZERO);
                spr->setPosition(col * tileSize + gameSize.width * pos, tileSize * row);
                this->addChild(spr);

                groundTiles.push_back(spr->getBoundingBox());
            }
            auto spr1 = Sprite::createWithSpriteFrame(tiles[23]);
            spr1->setAnchorPoint(Vec2::ZERO);
            spr1->setPosition((gameCol / 2 - 4) * tileSize + gameSize.width * pos, tileSize * 3);
            this->addChild(spr1);
            rearWallTiles.push_back(spr1->getBoundingBox());
            for (int i = 0; i < 3; i++)
            {
                auto spr = Sprite::createWithSpriteFrame(tiles[16]);
                spr->setAnchorPoint(Vec2::ZERO);
                spr->setPosition((gameCol / 2 - 4) * tileSize + gameSize.width * pos, tileSize * i);
                this->addChild(spr);
                wallTiles.push_back(spr->getBoundingBox());
			}
            for (int col = gameCol / 2 + 5; col < gameCol; col++)
            {
                int row = 0;
                for (row; row < 3; row++)
                {
                    auto spr = Sprite::createWithSpriteFrame(tiles[15]);
                    spr->setAnchorPoint(Vec2::ZERO);
                    spr->setPosition(col * tileSize + gameSize.width * pos, tileSize * row);
                    this->addChild(spr);
                }

                auto spr = Sprite::createWithSpriteFrame(tiles[22]);
                spr->setAnchorPoint(Vec2::ZERO);
                spr->setPosition(col * tileSize + gameSize.width * pos, tileSize * row);
                this->addChild(spr);

                groundTiles.push_back(spr->getBoundingBox());
            }
            auto spr2 = Sprite::createWithSpriteFrame(tiles[21]);
            spr2->setAnchorPoint(Vec2::ZERO);
            spr2->setPosition((gameCol / 2 + 4) * tileSize + gameSize.width * pos, tileSize * 3);
            this->addChild(spr2);
            rearWallTiles.push_back(spr2->getBoundingBox());
            for (int i = 0; i < 3; i++)
            {
                auto spr = Sprite::createWithSpriteFrame(tiles[14]);
                spr->setAnchorPoint(Vec2::ZERO);
                spr->setPosition((gameCol / 2 + 4) * tileSize + gameSize.width * pos, tileSize * i);
                this->addChild(spr);
                wallTiles.push_back(spr->getBoundingBox());
            }
            auto spr3 = Sprite::createWithSpriteFrame(tiles[24]);
            spr3->setAnchorPoint(Vec2::ZERO);
            spr3->setPosition((gameCol / 2) * tileSize + gameSize.width * pos, tileSize * 3);
            this->addChild(spr3);
            rearWallTiles.push_back(spr3->getBoundingBox());
            for (int i = 0; i < 3; i++)
            {
                auto spr = Sprite::createWithSpriteFrame(tiles[17]);
                spr->setAnchorPoint(Vec2::ZERO);
                spr->setPosition((gameCol / 2) * tileSize + gameSize.width * pos, tileSize * i);
                this->addChild(spr);
                wallTiles.push_back(spr->getBoundingBox());
            }
            spr3->setPosition((gameCol / 2) * tileSize + gameSize.width * pos, tileSize * 3);

            auto coin = Coin::create(Vec2((gameCol / 2) * tileSize + 16 + gameSize.width * pos, tileSize * 8));
            this->addChild(coin);
            break;
        }
        case 3:
        {
            for (int col = 0; col <= 5; col++)
            {
                auto spr = Sprite::createWithSpriteFrame(tiles[22]);
                spr->setAnchorPoint(Vec2::ZERO);
                spr->setPosition(col * tileSize + gameSize.width * pos, tileSize * 3);
                this->addChild(spr);

                groundTiles.push_back(spr->getBoundingBox());
            }

            auto spr1 = Sprite::createWithSpriteFrame(tiles[13]);
            spr1->setAnchorPoint(Vec2::ZERO);
            spr1->setPosition(6 * tileSize + gameSize.width * pos, tileSize * 3);
            this->addChild(spr1);
            groundTiles.push_back(spr1->getBoundingBox());
            for (int i = 4; i < 6; i++)
            {
                auto spr = Sprite::createWithSpriteFrame(tiles[14]);
                spr->setAnchorPoint(Vec2::ZERO);
                spr->setPosition(6 * tileSize + gameSize.width * pos, tileSize * i);
                this->addChild(spr);
                wallTiles.push_back(spr->getBoundingBox());
            }
            auto spr3 = Sprite::createWithSpriteFrame(tiles[21]);
            spr3->setAnchorPoint(Vec2::ZERO);
            spr3->setPosition(6 * tileSize + gameSize.width * pos, tileSize * 6);
            this->addChild(spr3);
            rearWallTiles.push_back(spr3->getBoundingBox());
            auto spr4 = Sprite::createWithSpriteFrame(tiles[22]);
            spr4->setAnchorPoint(Vec2::ZERO);
            spr4->setPosition(7 * tileSize + gameSize.width * pos, tileSize * 6);
            this->addChild(spr4);
            groundTiles.push_back(spr4->getBoundingBox());
            for (int i = 3; i < 6; i++)
            {
                auto spr = Sprite::createWithSpriteFrame(tiles[15]);
                spr->setAnchorPoint(Vec2::ZERO);
                spr->setPosition(7 * tileSize + gameSize.width * pos, tileSize * i);
                this->addChild(spr);
            }
            auto spr5 = Sprite::createWithSpriteFrame(tiles[11]);
            spr5->setAnchorPoint(Vec2::ZERO);
            spr5->setPosition(8 * tileSize + gameSize.width * pos, tileSize * 3);
            this->addChild(spr5);
            groundTiles.push_back(spr5->getBoundingBox());
            for (int i = 4; i < 6; i++)
            {
                auto spr = Sprite::createWithSpriteFrame(tiles[16]);
                spr->setAnchorPoint(Vec2::ZERO);
                spr->setPosition(8 * tileSize + gameSize.width * pos, tileSize * i);
                this->addChild(spr);
                wallTiles.push_back(spr->getBoundingBox());
            }
            auto spr6 = Sprite::createWithSpriteFrame(tiles[23]);
            spr6->setAnchorPoint(Vec2::ZERO);
            spr6->setPosition(8 * tileSize + gameSize.width * pos, tileSize * 6);
            this->addChild(spr6);
            rearWallTiles.push_back(spr6->getBoundingBox());

            for (int col = gameCol - 5; col < gameCol; col++)
            {
                auto spr = Sprite::createWithSpriteFrame(tiles[22]);
                spr->setAnchorPoint(Vec2::ZERO);
                spr->setPosition(col * tileSize + gameSize.width * pos, tileSize * 3);
                this->addChild(spr);

                groundTiles.push_back(spr->getBoundingBox());
            }
            auto spr2 = Sprite::createWithSpriteFrame(tiles[22]);
            spr2->setAnchorPoint(Vec2::ZERO);
            spr2->setPosition((gameCol - 6) * tileSize + gameSize.width * pos, tileSize * 3);
            this->addChild(spr2);
            groundTiles.push_back(spr2->getBoundingBox());

            auto spr7 = Sprite::createWithSpriteFrame(tiles[13]);
            spr7->setAnchorPoint(Vec2::ZERO);
            spr7->setPosition((gameCol - 9)* tileSize + gameSize.width * pos, tileSize * 3);
            this->addChild(spr7);
            groundTiles.push_back(spr7->getBoundingBox());
            for (int i = 4; i < 6; i++)
            {
                auto spr = Sprite::createWithSpriteFrame(tiles[14]);
                spr->setAnchorPoint(Vec2::ZERO);
                spr->setPosition((gameCol - 9)* tileSize + gameSize.width * pos, tileSize * i);
                this->addChild(spr);
                wallTiles.push_back(spr->getBoundingBox());
            }
            auto spr8 = Sprite::createWithSpriteFrame(tiles[21]);
            spr8->setAnchorPoint(Vec2::ZERO);
            spr8->setPosition((gameCol - 9)* tileSize + gameSize.width * pos, tileSize * 6);
            this->addChild(spr8);
            rearWallTiles.push_back(spr8->getBoundingBox());
            auto spr9 = Sprite::createWithSpriteFrame(tiles[22]);
            spr9->setAnchorPoint(Vec2::ZERO);
            spr9->setPosition((gameCol - 8) * tileSize + gameSize.width * pos, tileSize * 6);
            this->addChild(spr9);
            groundTiles.push_back(spr9->getBoundingBox());
            for (int i = 3; i < 6; i++)
            {
                auto spr = Sprite::createWithSpriteFrame(tiles[15]);
                spr->setAnchorPoint(Vec2::ZERO);
                spr->setPosition((gameCol - 8) * tileSize + gameSize.width * pos, tileSize * i);
                this->addChild(spr);
            }
            auto spr10 = Sprite::createWithSpriteFrame(tiles[11]);
            spr10->setAnchorPoint(Vec2::ZERO);
            spr10->setPosition((gameCol - 7)* tileSize + gameSize.width * pos, tileSize * 3);
            this->addChild(spr10);
            groundTiles.push_back(spr10->getBoundingBox());
            for (int i = 4; i < 6; i++)
            {
                auto spr = Sprite::createWithSpriteFrame(tiles[16]);
                spr->setAnchorPoint(Vec2::ZERO);
                spr->setPosition((gameCol - 7)* tileSize + gameSize.width * pos, tileSize * i);
                this->addChild(spr);
                wallTiles.push_back(spr->getBoundingBox());
            }
            auto spr11 = Sprite::createWithSpriteFrame(tiles[23]);
            spr11->setAnchorPoint(Vec2::ZERO);
            spr11->setPosition((gameCol - 7)* tileSize + gameSize.width * pos, tileSize * 6);
            this->addChild(spr11);
            rearWallTiles.push_back(spr11->getBoundingBox());

            for (int col = 9; col < gameCol - 9; col++)
            {
                auto spr = Sprite::createWithSpriteFrame(tiles[22]);
                spr->setAnchorPoint(Vec2::ZERO);
                spr->setPosition(col * tileSize + gameSize.width * pos, tileSize * 3);
                this->addChild(spr);

                groundTiles.push_back(spr->getBoundingBox());
            }

            for (int col = 0; col < gameCol; col++)
            {
                for (int row = 0; row < 3; row++)
                {
                    auto spr = Sprite::createWithSpriteFrame(tiles[15]);
                    spr->setAnchorPoint(Vec2::ZERO);
                    spr->setPosition(col * tileSize + gameSize.width * pos, tileSize * row);
                    this->addChild(spr);
                }
            }

            auto enemy = Enemy::createEnemy(Vec2(gameSize.width / 2 + gameSize.width * pos, 300), 100.0f);
            enemy->setPlayer(player);
            this->addChild(enemy, 1);
            break;
        }
        case 4:
        {
            for (int col = 0; col <= 5; col++)
            {
                int row = 0;
                for (row; row < 3; row++)
                {
                    auto spr = Sprite::createWithSpriteFrame(tiles[15]);
                    spr->setAnchorPoint(Vec2::ZERO);
                    spr->setPosition(col * tileSize + gameSize.width * pos, tileSize * row);
                    this->addChild(spr);
                }

                auto spr = Sprite::createWithSpriteFrame(tiles[22]);
                spr->setAnchorPoint(Vec2::ZERO);
                spr->setPosition(col * tileSize + gameSize.width * pos, tileSize * row);
                this->addChild(spr);

                groundTiles.push_back(spr->getBoundingBox());
            }
            auto spr1 = Sprite::createWithSpriteFrame(tiles[23]);
            spr1->setAnchorPoint(Vec2::ZERO);
            spr1->setPosition(6 * tileSize + gameSize.width * pos, tileSize * 3);
            this->addChild(spr1);
            rearWallTiles.push_back(spr1->getBoundingBox());
            for (int i = 0; i < 3; i++)
            {
                auto spr = Sprite::createWithSpriteFrame(tiles[16]);
                spr->setAnchorPoint(Vec2::ZERO);
                spr->setPosition(6 * tileSize + gameSize.width * pos, tileSize * i);
                this->addChild(spr);
                wallTiles.push_back(spr->getBoundingBox());
            }

            for (int col = 11; col < gameCol - 9; col++)
            {
                int row = 0;
                for (row; row < 3; row++)
                {
                    auto spr = Sprite::createWithSpriteFrame(tiles[15]);
                    spr->setAnchorPoint(Vec2::ZERO);
                    spr->setPosition(col * tileSize + gameSize.width * pos, tileSize * row);
                    this->addChild(spr);
                }

                auto spr = Sprite::createWithSpriteFrame(tiles[22]);
                spr->setAnchorPoint(Vec2::ZERO);
                spr->setPosition(col * tileSize + gameSize.width * pos, tileSize * row);
                this->addChild(spr);

                groundTiles.push_back(spr->getBoundingBox());
            }
            auto spr2 = Sprite::createWithSpriteFrame(tiles[21]);
            spr2->setAnchorPoint(Vec2::ZERO);
            spr2->setPosition(10 * tileSize + gameSize.width * pos, tileSize * 3);
            this->addChild(spr2);
            rearWallTiles.push_back(spr2->getBoundingBox());
            for (int i = 0; i < 3; i++)
            {
                auto spr = Sprite::createWithSpriteFrame(tiles[14]);
                spr->setAnchorPoint(Vec2::ZERO);
                spr->setPosition(10 * tileSize + gameSize.width * pos, tileSize * i);
                this->addChild(spr);
                wallTiles.push_back(spr->getBoundingBox());
            }
            auto spr4 = Sprite::createWithSpriteFrame(tiles[23]);
            spr4->setAnchorPoint(Vec2::ZERO);
            spr4->setPosition((gameCol - 10)* tileSize + gameSize.width * pos, tileSize * 3);
            this->addChild(spr4);
            rearWallTiles.push_back(spr4->getBoundingBox());
            for (int i = 0; i < 3; i++)
            {
                auto spr = Sprite::createWithSpriteFrame(tiles[16]);
                spr->setAnchorPoint(Vec2::ZERO);
                spr->setPosition((gameCol - 10)* tileSize + gameSize.width * pos, tileSize * i);
                this->addChild(spr);
                wallTiles.push_back(spr->getBoundingBox());
            }

            for (int col = gameCol - 5; col < gameCol; col++)
            {
                int row = 0;
                for (row; row < 3; row++)
                {
                    auto spr = Sprite::createWithSpriteFrame(tiles[15]);
                    spr->setAnchorPoint(Vec2::ZERO);
                    spr->setPosition(col * tileSize + gameSize.width * pos, tileSize * row);
                    this->addChild(spr);
                }

                auto spr = Sprite::createWithSpriteFrame(tiles[22]);
                spr->setAnchorPoint(Vec2::ZERO);
                spr->setPosition(col * tileSize + gameSize.width * pos, tileSize * row);
                this->addChild(spr);

                groundTiles.push_back(spr->getBoundingBox());
            }
            auto spr3 = Sprite::createWithSpriteFrame(tiles[21]);
            spr3->setAnchorPoint(Vec2::ZERO);
            spr3->setPosition((gameCol - 6) * tileSize + gameSize.width * pos, tileSize * 3);
            this->addChild(spr3);
            rearWallTiles.push_back(spr3->getBoundingBox());
            for (int i = 0; i < 3; i++)
            {
                auto spr = Sprite::createWithSpriteFrame(tiles[14]);
                spr->setAnchorPoint(Vec2::ZERO);
                spr->setPosition((gameCol - 6)* tileSize + gameSize.width * pos, tileSize * i);
                this->addChild(spr);
                wallTiles.push_back(spr->getBoundingBox());
            }

            auto enemy = Enemy::createEnemy(Vec2(gameSize.width / 2 + gameSize.width * pos, 300), 100.0f);
            enemy->setPlayer(player);
            this->addChild(enemy, 1);
            break;
        }
        case 5:
        {
            for (int col = 0; col < gameCol; col++)
            {
                for (int row = 0; row < 3; row++)
                {
                    auto spr = Sprite::createWithSpriteFrame(tiles[15]);
                    spr->setAnchorPoint(Vec2::ZERO);
                    spr->setPosition(col * tileSize + gameSize.width * pos, tileSize * row);
                    this->addChild(spr);
                }
            }
            for (int col = 0; col < 5; col++)
            {
                auto spr = Sprite::createWithSpriteFrame(tiles[22]);
                spr->setAnchorPoint(Vec2::ZERO);
                spr->setPosition(col* tileSize + gameSize.width * pos, tileSize* 3);
                this->addChild(spr);
				groundTiles.push_back(spr->getBoundingBox());
            }
            for (int col = gameCol - 5; col < gameCol; col++)
            {
                auto spr = Sprite::createWithSpriteFrame(tiles[22]);
                spr->setAnchorPoint(Vec2::ZERO);
                spr->setPosition(col * tileSize + gameSize.width * pos, tileSize * 3);
                this->addChild(spr);
                groundTiles.push_back(spr->getBoundingBox());
            }
            auto spr1 = Sprite::createWithSpriteFrame(tiles[21]);
            spr1->setAnchorPoint(Vec2::ZERO);
            spr1->setPosition(5 * tileSize + gameSize.width * pos, tileSize * 6 );
            this->addChild(spr1);
            rearWallTiles.push_back(spr1->getBoundingBox());
            for (int i = 4; i < 6; i++)
            {
                auto spr = Sprite::createWithSpriteFrame(tiles[14]);
                spr->setAnchorPoint(Vec2::ZERO);
                spr->setPosition(5 * tileSize + gameSize.width * pos, tileSize * i);
                this->addChild(spr);
                wallTiles.push_back(spr->getBoundingBox());
            }
            auto spr3 = Sprite::createWithSpriteFrame(tiles[23]);
            spr3->setAnchorPoint(Vec2::ZERO);
            spr3->setPosition((gameCol - 6)* tileSize + gameSize.width * pos, tileSize * 6);
            this->addChild(spr3);
            rearWallTiles.push_back(spr3->getBoundingBox());
            for (int i = 4; i < 6; i++)
            {
                auto spr = Sprite::createWithSpriteFrame(tiles[16]);
                spr->setAnchorPoint(Vec2::ZERO);
                spr->setPosition((gameCol - 6) * tileSize + gameSize.width * pos, tileSize * i);
                this->addChild(spr);
                wallTiles.push_back(spr->getBoundingBox());
            }
            for (int col = 6; col < gameCol - 6; col++)
            {
                for (int row = 4; row < 6; row++)
                {
                    auto spr = Sprite::createWithSpriteFrame(tiles[15]);
                    spr->setAnchorPoint(Vec2::ZERO);
                    spr->setPosition(col * tileSize + gameSize.width * pos, tileSize * row);
                    this->addChild(spr);
                }
            }
            for (int col = 6; col < gameCol - 6; col++)
            {
                auto spr = Sprite::createWithSpriteFrame(tiles[22]);
                spr->setAnchorPoint(Vec2::ZERO);
                spr->setPosition(col * tileSize + gameSize.width * pos, tileSize * 6);
                this->addChild(spr);
                groundTiles.push_back(spr->getBoundingBox());
            }
            auto spr2 = Sprite::createWithSpriteFrame(tiles[13]);
            spr2->setAnchorPoint(Vec2::ZERO);
            spr2->setPosition(5 * tileSize + gameSize.width * pos, tileSize * 3);
            this->addChild(spr2);
            groundTiles.push_back(spr2->getBoundingBox());
            auto spr5 = Sprite::createWithSpriteFrame(tiles[11]);
            spr5->setAnchorPoint(Vec2::ZERO);
            spr5->setPosition((gameCol - 6)* tileSize + gameSize.width * pos, tileSize * 3);
            this->addChild(spr5);
            groundTiles.push_back(spr5->getBoundingBox());
            for (int col = 6; col < gameCol - 6; col++)
            {
                auto spr = Sprite::createWithSpriteFrame(tiles[15]);
                spr->setAnchorPoint(Vec2::ZERO);
                spr->setPosition(col * tileSize + gameSize.width * pos, tileSize * 3);
                this->addChild(spr);
            }
            auto enemy = Enemy::createEnemy(Vec2(gameSize.width / 2 + gameSize.width * pos, 300), 100.0f);
            enemy->setPlayer(player);
            this->addChild(enemy, 1);
            break;
        }
        case 6:
        {

            break;
        }
        case 7:
        {

            break;
        }
        case 8:
        {

            break;
        }
    }

}