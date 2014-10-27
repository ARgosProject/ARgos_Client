#include "AudioManager.h"
#include "Log.h"
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>

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

  void AudioManager::preload(const std::string& sound_name, const std::string& file_name) {
    _soundsMap[sound_name] = Mix_LoadWAV(file_name.c_str());

    if(_soundsMap[sound_name] == NULL) {
      Log::error("No se pudo cargar el sonido: " + file_name);
      SDL_Quit();
    }
  }

  void AudioManager::play(const std::string& sound_name) {
    Mix_PlayChannel(-1, _soundsMap[sound_name], 0);
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

  void AudioManager::volume(const std::string& sound_name, int val) {
    Mix_VolumeChunk(_soundsMap[sound_name], val);
  }

  void AudioManager::volumeAll(int val) {
    Mix_Volume(-1, val);
  }

  int AudioManager::isPlaying() {
    return Mix_Playing(-1);
  }

}
