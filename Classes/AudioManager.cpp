#include "AudioManager.h"

USING_NS_CC;

AudioManager* AudioManager::getInstance()
{
    static AudioManager instance;
    return &instance;
}

void AudioManager::playBGM(const std::string& file, bool loop)
{
    stopBGM();
    currentBGMId = AudioEngine::play2d(file, loop, bgmVolume);
}

void AudioManager::stopBGM()
{
    if (currentBGMId != -1)
    {
        AudioEngine::stop(currentBGMId);
        currentBGMId = -1;
    }
}

void AudioManager::setBGMVolume(float volume)
{
    bgmVolume = volume;
    if (currentBGMId != -1)
        AudioEngine::setVolume(currentBGMId, bgmVolume);
}

void AudioManager::playSFX(const std::string& file)
{
    AudioEngine::play2d(file, false, sfxVolume);
}

void AudioManager::setSFXVolume(float volume)
{
    sfxVolume = volume;
}

void AudioManager::stopAll()
{
    AudioEngine::stopAll();
}