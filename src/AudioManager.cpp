#include "AudioManager.h"
#include "Log.h"
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <dirent.h>

namespace argosClient {

  AudioManager::AudioManager() {
    if(SDL_Init(SDL_INIT_AUDIO) < 0) {
      Log::error("No se pudo inicializar el sistema de audio");
      SDL_Quit();
    }

    atexit(SDL_Quit);

    // Inicializando SDL mixer...
    if(Mix_OpenAudio(16000, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 4096) < 0) {
      Log::error("No se pudo inicializar el mezclador de sonido");
      SDL_Quit();
    }

    atexit(Mix_CloseAudio);
  }

  AudioManager::~AudioManager() {

  }

  void AudioManager::setSoundsPath(const std::string& path) {
    _soundsPath = path;
  }

  void AudioManager::preload(const std::string& file_name) {
    _soundsMap[file_name] = Mix_LoadWAV((_soundsPath + file_name).c_str());

    if(_soundsMap[file_name] == NULL) {
      Log::error("Error loading the sound: '" + _soundsPath + file_name + "'");
      SDL_Quit();
    }

    Log::success("Sound '" + _soundsPath + file_name + "' successfully loaded");
  }

  void AudioManager::preloadAll() {
    DIR *dir;
    dirent *ent;

    if((dir = opendir(_soundsPath.c_str())) != NULL) {
      while((ent = readdir(dir)) != NULL) {
        if((strcmp(ent->d_name, ".") != 0) && (strcmp(ent->d_name, "..") != 0))
          preload(ent->d_name);
      }
      closedir(dir);
    }
    else {
      Log::error("There was an error loading the sounds from '" + _soundsPath + "'. Aborting");
      SDL_Quit();
    }
  }

  void AudioManager::play(const std::string& file_name) {
    if(_soundsMap.find(file_name) != _soundsMap.end()) {
      Mix_PlayChannel(-1, _soundsMap[file_name], 0);
    }
    else {
      preload(file_name);
      play(file_name);
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
