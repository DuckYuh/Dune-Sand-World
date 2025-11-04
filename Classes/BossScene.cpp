#include "BossScene.h"
#include "MainMenuScene.h"
#include "Player.h"
#include "Boss.h"
#include "Bullet.h"
#include "GameManager.h"
#include "AudioManager.h"

USING_NS_CC;

Scene* BossScene::createScene()
{
    return BossScene::create();
}

bool BossScene::init()
{
    if (!Scene::init())
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    drawBG();
    drawTileMap();

    AudioManager::getInstance()->playSFX("sounds/game-start.mp3");

    auto player = Player::createPlayer();
    player->setPosition(origin.x + 200, origin.y + 300);
    this->addChild(player, 1, "player");
    player->stop();

    auto boss = Boss::createBoss(Vec2(origin.x + 1000, origin.y + 300));
    boss->setPlayer(player);
    this->addChild(boss, 1, "boss");

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
    listener->onKeyPressed = [this, player, uiLayer, boss](EventKeyboard::KeyCode keyCode, Event* event)
        {
            cheatInput.push_back(keyCode);

            if (cheatInput.size() > cheatSequence.size())   cheatInput.erase(cheatInput.begin());

            if (cheatInput == cheatSequence)
            {
                cheatInput.clear();
				boss->kill();
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
                else if (uiLayer->getChildByName("WinBoard"))
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

    auto camera = Follow::create(player, Rect(0, 0, 1500, visibleSize.height));
    this->runAction(camera);

    showUI();

    scheduleUpdate();

    return true;
}

void BossScene::drawBG()
{
    auto sky = Director::getInstance()->getTextureCache()->addImage("images/Backgrounds/Desert Background Desert Layer 5.png");
    int skyW = sky->getPixelsWide();

    for (int i = 0; i < 2; i++)
    {
        auto spr = Sprite::createWithTexture(sky);
        spr->setAnchorPoint(Vec2::ZERO);
        spr->setScale(1.5f);

        float sprH = spr->getContentSize().height * spr->getScale();
        spr->setPosition(i * skyW * 1.5f, 720 - sprH);
        this->addChild(spr, -3);
    }

    auto sun = Sprite::create("images/Backgrounds/BG-sun.png");
    sun->setPosition(1000, 500);
    this->addChild(sun, -3);

    auto cloud = Director::getInstance()->getTextureCache()->addImage("images/Backgrounds/Desert Background Desert Layer 4.png");
    int cloudW = cloud->getPixelsWide();

    for (int i = 0; i < 2; i++)
    {
        auto spr = Sprite::createWithTexture(cloud);
        spr->setAnchorPoint(Vec2::ZERO);
        float sprH = spr->getContentSize().height;
        spr->setPosition(i * cloudW * 1.5f, 720 - sprH);
        this->addChild(spr, -3);
    }

    auto bg = Director::getInstance()->getTextureCache()->addImage("images/Backgrounds/Desert Background Desert Layer 1.png");
    int bgW = bg->getPixelsWide();

    for (int i = 0; i < 2; i++)
    {
        auto spr = Sprite::createWithTexture(bg);
        spr->setAnchorPoint(Vec2::ZERO);
        spr->setScale(1.5f);

        spr->setPosition(i * bgW * 1.5f, 0);
        this->addChild(spr, -2);
    }
}

void BossScene::drawTileMap()
{
    auto gameSize = Director::getInstance()->getVisibleSize();

    auto texture = Director::getInstance()->getTextureCache()->addImage("images/Tileset/castle.png");

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
    int gameCol = gameSize.width / tileSize + 10;

    for (int col = 0; col < gameCol; col++)
    {
        int row = 0;
        for (row; row < 2; row++)
        {
            auto spr = Sprite::createWithSpriteFrame(tiles[27]);
            spr->setAnchorPoint(Vec2::ZERO);
            spr->setPosition(col * tileSize, tileSize * row);
            this->addChild(spr);
        }

        auto spr = Sprite::createWithSpriteFrame(tiles[38]);
        spr->setAnchorPoint(Vec2::ZERO);
        spr->setPosition(col * tileSize, tileSize * row);
        this->addChild(spr);

        groundTiles.push_back(spr->getBoundingBox());
    }
}

void BossScene::showUI()
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

void BossScene::pauseGame()
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
        CC_CALLBACK_1(BossScene::menuResumeCallback, this));
    resumeItem->setPosition(Vec2(panel->getContentSize().width / 2 - 200, panel->getContentSize().height / 2 - 100));
    auto resumeLabel = Label::createWithTTF("Resume", "fonts/GomePixel-DYJX1.otf", 36);
    resumeLabel->setPosition(resumeItem->getContentSize() / 2);
    resumeLabel->setTextColor(Color4B::WHITE);
    resumeLabel->enableOutline(Color4B::BLACK, 2);
    resumeItem->addChild(resumeLabel);

    auto menuItem = MenuItemImage::create("images/ui/Button.png",
        "images/ui/ButtonPressed.png",
        CC_CALLBACK_1(BossScene::menuMainCallback, this));
    menuItem->setPosition(Vec2(panel->getContentSize().width / 2, panel->getContentSize().height / 2 - 100));
    auto menuLabel = Label::createWithTTF("Menu", "fonts/GomePixel-DYJX1.otf", 36);
    menuLabel->setPosition(menuItem->getContentSize() / 2);
    menuLabel->setTextColor(Color4B::WHITE);
    menuLabel->enableOutline(Color4B::BLACK, 2);
    menuItem->addChild(menuLabel);

    auto exitItem = MenuItemImage::create("images/ui/Button.png",
        "images/ui/ButtonPressed.png",
        CC_CALLBACK_1(BossScene::menuCloseCallback, this));
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

void BossScene::showEndGameBoard()
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

void BossScene::winBroad()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto uiLayer = this->getChildByName("UILayer");
    if (!uiLayer) return;

    if (uiLayer->getChildByName("WinBoard")) return;

    Director::getInstance()->pause();

    auto popupBg = LayerColor::create(Color4B(0, 0, 0, 192));
    popupBg->setContentSize(visibleSize);
    popupBg->setName("WinBoard");
    uiLayer->addChild(popupBg, 1000);

    auto panel = Sprite::create("images/ui/OptionBoard.png");
    panel->setPosition(visibleSize.width / 2, visibleSize.height / 2);
    popupBg->addChild(panel);

    auto gameOverLabel = Label::createWithTTF("Congratulation!!!", "fonts/GomePixel-DYJX1.otf", 48);
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

void BossScene::update(float delta)
{
    GameManager::getInstance()->addElapsedTime(delta);

    auto player = dynamic_cast<Player*>(this->getChildByName("player"));
    auto uiLayer = this->getChildByName("UILayer");
    auto boss = dynamic_cast<Boss*>(this->getChildByName("boss"));

    auto visibleSize = Director::getInstance()->getVisibleSize();

    if(player && uiLayer)
    {
        float offsetX = player->getPositionX() - visibleSize.width / 2;
        offsetX = clampf(offsetX, 0, 1500 - visibleSize.width);

        uiLayer->setPositionX(offsetX);
    }

    if (player && GameManager::getInstance()->getPlayerHealth() <= 0)
    {
        showEndGameBoard();
        return;
    }

    if (!boss || boss->getHealth() <= 0)
    {
        winBroad();
		return;
    }

    if (player->getPositionX() <= 16)
    {
        player->setPositionX(16);
    }
    if (player->getPositionX() >= 1500)
    {
        player->setPositionX(1500);
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
        if (boss && boss->getBoundingBox().intersectsRect(rect))
        {
            boss->setPositionY(rect.getMaxY() + 16);
            boss->onGround();
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

        if (boss && bullet->getBoundingBox().intersectsRect(boss->getBoundingBox()))
        {
            bullet->removeFromParent();
            if (boss->getHealth() > 1)
            {
                boss->beHitted();
            }
            else
            {
                boss->runAction(Sequence::create(
                    FadeOut::create(0.5f),
                    CallFunc::create([boss]() {
                        boss->removeFromParent();
                        }),
                    nullptr
                ));
            }
        }
    }
}

void BossScene::menuResumeCallback(Ref* pSender)
{
    auto uiLayer = this->getChildByName("UILayer");
    uiLayer->removeChildByName("PauseMenu");
    Director::getInstance()->resume();
}

void BossScene::menuMainCallback(Ref* pSender)
{
    Director::getInstance()->resume();
    GameManager::getInstance()->fullHeal();
    GameManager::getInstance()->setElapsedTime(0);
    auto menuScene = MainMenuScene::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(1.0f, menuScene));
}

void BossScene::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();
}