#include "AudioManager.h"
#include "Log.h"
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <dirent.h>

namespace argosClient {

  AudioManager::AudioManager() {
    if(SDL_Init(SDL_INIT_AUDIO) < 0) {
      Log::error("Could not init audio system.");
      SDL_Quit();
    }

    atexit(SDL_Quit);

    // Inicializando SDL mixer...
    if(Mix_OpenAudio(16000, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 4096) < 0) {
      Log::error("Could not init audio mixer.");
      SDL_Quit();
    }

    atexit(Mix_CloseAudio);
  }

  AudioManager::~AudioManager() {
    for(auto& pair : _soundsMap)
      Mix_FreeChunk(pair.second);
  }

  void AudioManager::setSoundsPath(const std::string& path) {
    _soundsPath = path;
  }

  void AudioManager::preload(const std::string& file_name) {
    _soundsMap[file_name] = Mix_LoadWAV((_soundsPath + file_name).c_str());

    if(_soundsMap[file_name] == nullptr) {
      Log::error("Loading the sound: '" + _soundsPath + file_name + "'.");
    }
    else {
      Log::success("Sound '" + _soundsPath + file_name + "' successfully loaded.");
    }
  }

  void AudioManager::preloadAll() {
    DIR *dir;
    dirent *ent;

    if((dir = opendir(_soundsPath.c_str())) != nullptr) {
      while((ent = readdir(dir)) != nullptr) {
        if((strcmp(ent->d_name, ".") != 0) && (strcmp(ent->d_name, "..") != 0))
          preload(ent->d_name);
      }
      closedir(dir);
    }
    else {
      Log::error("There was an error loading the sounds from '" + _soundsPath + "'. ");
    }
  }

  void AudioManager::play(const std::string& file_name, int loops) {
    if(_soundsMap.find(file_name) != _soundsMap.end()) {
      Log::info("Playing sound: '" + file_name + "'.");
      Mix_PlayChannel(-1, _soundsMap[file_name], loops);
    }
    else {
      preload(file_name);
      play(file_name, loops);
    }
  }

  void AudioManager::pause() {
    Mix_Pause(-1);
  }

  void AudioManager::resume() {
    Mix_Resume(-1);
  }

  void AudioManager::stop() {
    Mix_HaltChannel(-1);
  }

  void AudioManager::volume(const std::string& file_name, int val) {
    Mix_VolumeChunk(_soundsMap[file_name], val);
  }

  void AudioManager::volumeAll(int val) {
    Mix_Volume(-1, val);
  }

  int AudioManager::isPlaying() {
    return Mix_Playing(-1);
  }

}
