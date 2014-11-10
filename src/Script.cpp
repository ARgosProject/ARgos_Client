#include "Script.h"
#include "ScriptManager.h"
#include "Log.h"

#include <fstream>

namespace argosClient {

  Script::Script(ScriptManager& scriptManager)
    : _scriptManager(scriptManager), _priorScript(NULL), _nextScript(NULL), _secondsToWait(0),
      _currentSentence(0) {

  }

  Script::~Script() {
    _properties.clear();
    _priorScript = NULL;
    _nextScript = NULL;
  }

  const std::string& Script::getProperty(const std::string& key) {
    return _properties[key];
  }

  void Script::setProperty(const std::string& key, const std::string& value) {
    _properties[key] = value;
  }

  void Script::load(const std::string& path) {
    if(!_sentences.empty())
      _sentences.clear();

    std::ifstream file;
    try {
      file.open(path);

      if(!file)
        throw std::ios::failure("There was an error opening the script file: " + path);

      std::string line;
      while(std::getline(file, line)) {
        switch(line[0]) {
        case '!':
          // Stop parsing
          return;
        case '#':
          // Comment - Ignore it
          break;
        case '>':
          // Command - Process it
          _sentences.push_back(ScriptSentence(line));
          break;
        }
      }
    } catch(const std::exception& e) {
      Log::error(e.what());
    }
  }

  Script* Script::update() {
    if(_nextScript) {
      _nextScript->_priorScript = this;
      return _nextScript;
    }

    if(_secondsToWait != 0) {
      if(_timer.getSeconds() >= _secondsToWait) {
        _secondsToWait = 0;
        return runOneSentence();
      }
    }

    return runOneSentence();
  }

  Script* Script::runOneSentence() {
    Script* toReturn = NULL;

    if(_sentences.empty())
      return toReturn;

    _sentences[_currentSentence].execute(*this, _scriptManager);

    if(_currentSentence < _sentences.size() - 1) {
      ++_currentSentence;
      toReturn = this;
    }
    else {
      _currentSentence = 0;
    }

    return toReturn;
  }

}
