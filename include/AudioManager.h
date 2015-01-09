#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include <map>
#include <string>
#include "Singleton.h"

class Mix_Chunk;

namespace argosClient {

  /**
   * The audio manager of the system
   */
  class AudioManager : public Singleton<AudioManager> {
  public:
    /**
     * Constructs a new audio manager
     */
    AudioManager();

    /**
     * Destroys the audio manager
     */
    ~AudioManager();

    /**
     * Sets the path where the audio manager should load the sound files
     * @param path The path directory where the sound files are located
     */
    void setSoundsPath(const std::string& path);

    /**
     * Loads an audio file in memory
     * @param file_name The file to preload
     */
    void preload(const std::string& file_name);

    /**
     * Loads all files from the path in memory
     */
    void preloadAll();

    /**
     * Plays the specified audio file
     * @param file_name The file to play
     * @param loops The number of times to play it again (-1 infinite)
     */
    void play(const std::string& file_name, int loops = 0);

    /**
     * Pauses the current playing sound
     */
    void pause();

    /**
     * Resumes the current playing sound
     */
    void resume();

    /**
     * Stops the current playing sound
     */
    void stop();

    /**
     * Handles the volume of the specified sound
     * @param file_name The sound file to change volume
     * @param vale The new volume value
     */
    void volume(const std::string& file_name, int val);

    /**
     * Handles the volume of all the sounds
     * @param vale The new volume value
     */
    void volumeAll(int val);

    /**
     * Checks whether the system is playing some sound
     * @return true if the system is playing some sound, false otherwise
     */
    int isPlaying();

  private:
    std::map<std::string, Mix_Chunk*> _soundsMap; ///< An associative list of sounds indexed by its names
    std::string _soundsPath; ///< The directory path where the sounds are located for later loading
  };

}
#endif
