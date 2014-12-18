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

    void setSoundsPath(const std::string& path);
    void preload(const std::string& file_name);
    void preloadAll();

    void play(const std::string& file_name, int loops = 0);
    void pause();
    void resume();
    void stop();

    void volume(const std::string& file_name, int val);
    void volumeAll(int val);

    int isPlaying();

  private:
    std::map<std::string, Mix_Chunk*> _soundsMap;
    std::string _soundsPath;
  };

}
#endif
