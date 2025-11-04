#ifndef __MAIN_MENU_SCENE_H__
#define __MAIN_MENU_SCENE_H__

#include "cocos2d.h"

class MainMenuScene : public cocos2d::Scene
{
private:
    bool isFullscreen = false;
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    ~MainMenuScene();

    void menuCloseCallback(cocos2d::Ref* pSender);
    void menuOptionCallback(cocos2d::Ref* pSender);
    void menuAboutCallback(cocos2d::Ref* pSender);
    void menuPlayCallback(cocos2d::Ref* pSender);

    // implement the "static create()" method manually
    CREATE_FUNC(MainMenuScene);
};

#endif // __MAIN_MENU_SCENE_H__