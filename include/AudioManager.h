#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include <map>
#include <string>
#include "Singleton.h"

class Mix_Chunk;

namespace argosClient {

  class AudioManager : public Singleton<AudioManager> {
  public:
    AudioManager();
    ~AudioManager();

    void preload(const std::string& sound_name, const std::string& file_name);

    void play(const std::string& sound_name);
    void pause();
    void resume();
    void stop();

    void volume(const std::string& sound_name, int val);
    void volumeAll(int val);

    int isPlaying();

  private:
    std::map<std::string, Mix_Chunk*> _soundsMap;
  };

}
#endif
