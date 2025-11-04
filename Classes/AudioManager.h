#ifndef __AUDIO_MANAGER_H__
#define __AUDIO_MANAGER_H__

#include "audio/include/AudioEngine.h"
#include "cocos2d.h"

class AudioManager
{
private:
    int currentBGMId = -1;
    float bgmVolume = 0.5f;
    float sfxVolume = 0.5f;

    AudioManager() = default; // private constructor

public:
    static AudioManager* getInstance();

    void playBGM(const std::string& file, bool loop = true);
    void stopBGM();
    void setBGMVolume(float volume);
    float getBGMVolume() const { return bgmVolume; }

    void playSFX(const std::string& file);
    void setSFXVolume(float volume);
    float getSFXVolume() const { return sfxVolume; }

    void stopAll();
};

#endif //__AUDIO_MANAGER_H__