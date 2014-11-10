#ifndef SCRIPT_H
#define SCRIPT_H

#include <map>
#include <string>

#include "Timer.h"
#include "ScriptSentence.h"

namespace argosClient {

  class ScriptManager;

  class Script {
  public:
    Script(ScriptManager& scriptManager);
    ~Script();

    const std::string& getProperty(const std::string& key);
    void setProperty(const std::string& key, const std::string& value);

    void load(const std::string& path);
    Script* update();

  private:
    Script* runOneSentence();

  private:
    ScriptManager& _scriptManager;

    std::vector<ScriptSentence> _sentences;
    Script* _priorScript;
    Script* _nextScript;
    std::map<std::string, std::string> _properties;
    Timer _timer;
    unsigned int _secondsToWait;
    unsigned int _currentSentence;
  };

}

#endif
