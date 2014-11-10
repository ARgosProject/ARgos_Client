#ifndef SCRIPTMANAGER_H
#define SCRIPTMANAGER_H

#include <vector>
#include <map>
#include <string>
#include <functional>

#include "Singleton.h"
#include "ScriptFunction.h"

namespace argosClient {

  class Script;

  class ScriptManager : public Singleton<ScriptManager> {
    friend class ScriptSentence;

  public:
    ScriptManager();
    ~ScriptManager();

    void loadScripts(const std::string& path);
    void runScript(const std::string& name);
    void update();

  private:
    void fillHandlers();

  private:
    std::map<std::string, ScriptFunction*> _handlers;
    std::map<std::string, Script*> _scripts;
    Script* _currentScript;
  };

}

#endif
