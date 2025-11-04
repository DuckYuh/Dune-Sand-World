#include "MainMenuScene.h"
#include "GameScene.h"
#include "AudioManager.h"
#include "ui/CocosGUI.h"

using namespace cocos2d::ui;

USING_NS_CC;

Scene* MainMenuScene::createScene()
{
    return MainMenuScene::create();
}

bool MainMenuScene::init()
{
    if (!Scene::init())
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto bg = Sprite::create("images/background.jpg");
    bg->setPosition(visibleSize.width / 2, visibleSize.height / 2);
    float scaleX = visibleSize.width / bg->getContentSize().width;
    float scaleY = visibleSize.height / bg->getContentSize().height;
    bg->setScaleX(scaleX);
    bg->setScaleY(scaleY);
    this->addChild(bg, -1);

    auto title = Label::createWithTTF("DUNE: SAND WORLD", "fonts/GomePixel-DYJX1.otf", 72);
    title->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height - 100));
    title->setTextColor(Color4B(255, 215, 0, 255));
    title->enableOutline(Color4B::BLACK, 4);
    this->addChild(title);

    AudioManager::getInstance()->playBGM("sounds/pauls-dream.mp3");

    auto playItem = MenuItemImage::create("images/ui/Button.png",
                                          "images/ui/ButtonPressed.png",
                                          CC_CALLBACK_1(MainMenuScene::menuPlayCallback, this));
    playItem->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    auto PlayLabel = Label::createWithTTF("Play", "fonts/GomePixel-DYJX1.otf", 36);
    PlayLabel->setPosition(playItem->getContentSize() / 2);
    PlayLabel->setTextColor(Color4B::WHITE);
    PlayLabel->enableOutline(Color4B::BLACK, 2);
    playItem->addChild(PlayLabel);

    auto optionItem = MenuItemImage::create("images/ui/Button.png",
                                            "images/ui/ButtonPressed.png",
                                            CC_CALLBACK_1(MainMenuScene::menuOptionCallback, this));
    optionItem->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 - 100));
    auto OptionLabel = Label::createWithTTF("Option", "fonts/GomePixel-DYJX1.otf", 36);
    OptionLabel->setPosition(optionItem->getContentSize() / 2);
    OptionLabel->setTextColor(Color4B::WHITE);
    OptionLabel->enableOutline(Color4B::BLACK, 2);
    optionItem->addChild(OptionLabel);

    auto aboutItem = MenuItemImage::create("images/ui/Button.png",
                                           "images/ui/ButtonPressed.png",
                                           CC_CALLBACK_1(MainMenuScene::menuAboutCallback, this));
    aboutItem->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 - 200));
    auto AboutLabel = Label::createWithTTF("About", "fonts/GomePixel-DYJX1.otf", 36);
    AboutLabel->setPosition(aboutItem->getContentSize() / 2);
    AboutLabel->setTextColor(Color4B::WHITE);
    AboutLabel->enableOutline(Color4B::BLACK, 2);
    aboutItem->addChild(AboutLabel);

    auto exitItem = MenuItemImage::create("images/ui/Button.png",
                                          "images/ui/ButtonPressed.png",
                                          CC_CALLBACK_1(MainMenuScene::menuCloseCallback, this));
    exitItem->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 - 300));
    auto ExitLabel = Label::createWithTTF("Exit", "fonts/GomePixel-DYJX1.otf", 36);
    ExitLabel->setPosition(exitItem->getContentSize() / 2);
    ExitLabel->setTextColor(Color4B::WHITE);
    ExitLabel->enableOutline(Color4B::BLACK, 2);
    exitItem->addChild(ExitLabel);

    auto menu = Menu::create(playItem, optionItem, aboutItem, exitItem, nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu);

    return true;
}

MainMenuScene::~MainMenuScene()
{
    AudioEngine::stopAll();
}

void MainMenuScene::menuPlayCallback(Ref* pSender)
{
    auto gameScene = GameScene::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(1.0f, gameScene));
}

void MainMenuScene::menuOptionCallback(Ref* pSender)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    auto popupBg = LayerColor::create(Color4B(0, 0, 0, 10));

    //popup
    popupBg->setContentSize(visibleSize);
    popupBg->setPosition(origin);
    popupBg->setName("OptionPopup");
    this->addChild(popupBg, 10);

    auto panel = Sprite::create("images/ui/OptionBoard.png");
    panel->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 - 50);
    popupBg->addChild(panel);

    auto optionLabel = Label::createWithTTF("Option", "fonts/GomePixel-DYJX1.otf", 28);
    optionLabel->setPosition(panel->getContentSize().width / 2, panel->getContentSize().height - 50);
    panel->addChild(optionLabel);

    // Slider
    auto slider = Slider::create();
    slider->loadBarTexture("images/ui/Slider.png");   // track
    slider->loadSlidBallTextures("images/ui/SlideBall.png"); // thumb
    slider->setPercent(50);
    slider->setPosition(Vec2(panel->getContentSize().width / 2, panel->getContentSize().height - 160));
    panel->addChild(slider);

    auto percentLabel = Label::createWithTTF("BGM", "fonts/GomePixel-DYJX1.otf", 24);
    percentLabel->setPosition(slider->getPositionX(), slider->getPositionY() + 40);
    panel->addChild(percentLabel);

    slider->addEventListener([=](Ref* sender, ui::Slider::EventType type) {
        if (type == ui::Slider::EventType::ON_PERCENTAGE_CHANGED) 
        {
            auto slider = dynamic_cast<ui::Slider*>(sender);
            float volume = slider->getPercent() / 100.0f;
            AudioManager::getInstance()->setBGMVolume(volume);
        }
        });

    auto sfxSlider = Slider::create();
    sfxSlider->loadBarTexture("images/ui/Slider.png");
    sfxSlider->loadSlidBallTextures("images/ui/SlideBall.png");
    sfxSlider->setPercent(AudioManager::getInstance()->getSFXVolume() * 100);
    sfxSlider->setPosition(Vec2(panel->getContentSize().width / 2, panel->getContentSize().height - 260));
    panel->addChild(sfxSlider);

    auto sfxLabel = Label::createWithTTF("SFX", "fonts/GomePixel-DYJX1.otf", 24);
    sfxLabel->setPosition(sfxSlider->getPositionX(), sfxSlider->getPositionY() + 40);
    panel->addChild(sfxLabel);

    sfxSlider->addEventListener([](Ref* sender, ui::Slider::EventType type) {
        if (type == ui::Slider::EventType::ON_PERCENTAGE_CHANGED)
        {
            auto slider = dynamic_cast<ui::Slider*>(sender);
            float volume = slider->getPercent() / 100.0f;
            AudioManager::getInstance()->setSFXVolume(volume);
        }
        });

    //fullscreen
    auto fullscreenLabel = Label::createWithTTF("Fullscreen", "fonts/GomePixel-DYJX1.otf", 24);
    fullscreenLabel->setPosition(panel->getContentSize().width / 2, panel->getContentSize().height - 320);
    panel->addChild(fullscreenLabel);

    auto checkbox = ui::CheckBox::create("images/ui/Checkbox.png",  
                                         "images/ui/CheckboxChecked.png");

    checkbox->setPosition(Vec2(panel->getContentSize().width / 2, panel->getContentSize().height - 360));
    checkbox->setSelected(isFullscreen);

    checkbox->addEventListener([=](Ref* sender, ui::CheckBox::EventType type) mutable {
        auto glview = Director::getInstance()->getOpenGLView();
        auto glviewImpl = dynamic_cast<GLViewImpl*>(glview);
        if (!glviewImpl) return;

        if (type == ui::CheckBox::EventType::SELECTED) {
            glviewImpl->setFullscreen();
            isFullscreen = true;
        }
        else if (type == ui::CheckBox::EventType::UNSELECTED) {
            glviewImpl->setWindowed(1280, 720);
            isFullscreen = false;
        }
        });

    panel->addChild(checkbox);

    //close
    auto closeItem = MenuItemLabel::create(
        Label::createWithTTF("Close", "fonts/GomePixel-DYJX1.otf", 28),
        [this](Ref* sender) {
            this->removeChildByName("OptionPopup");
        }
    );
    closeItem->setPosition(panel->getContentSize().width / 2, panel->getContentSize().height / 2 - 200);

    auto menu = Menu::create(closeItem, nullptr);
    menu->setPosition(Vec2::ZERO);
    panel->addChild(menu); 
}

void MainMenuScene::menuAboutCallback(Ref* pSender)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    auto popupBg = LayerColor::create(Color4B(0, 0, 0, 192));
    popupBg->setContentSize(visibleSize);
    popupBg->setPosition(origin);
    popupBg->setName("AboutPopup");
    this->addChild(popupBg, 10);

    auto panel = Sprite::create("images/ui/OptionBoard.png");
    panel->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 - 50);
    popupBg->addChild(panel);

    auto aboutLabel = Label::createWithTTF("Controls", "fonts/GomePixel-DYJX1.otf", 28);
    aboutLabel->setPosition(panel->getContentSize().width / 2, panel->getContentSize().height - 50);
    panel->addChild(aboutLabel);

    std::vector<std::string> controls = {
        "Move Left: Left Arrow",
        "Move Right: Right Arrow",
        "Jump: X",
        "Shoot: Z",
        "Pause: ESC"
    };

    float y = panel->getContentSize().height - 100;
    for (const auto& control : controls)
    {
        auto controlLabel = Label::createWithTTF(control, "fonts/GomePixel-DYJX1.otf", 24);
        controlLabel->setPosition(panel->getContentSize().width / 2, y);
        controlLabel->setTextColor(Color4B::WHITE);
        controlLabel->enableOutline(Color4B::BLACK, 1);
        panel->addChild(controlLabel);
        y -= 40;
    }

    auto closeItem = MenuItemLabel::create(
        Label::createWithTTF("Close", "fonts/GomePixel-DYJX1.otf", 28),
        [this](Ref* sender) {
            this->removeChildByName("AboutPopup");
        }
    );
    closeItem->setPosition(panel->getContentSize().width / 2, 50);

    auto menu = Menu::create(closeItem, nullptr);
    menu->setPosition(Vec2::ZERO);
    panel->addChild(menu);
}

void MainMenuScene::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();
}