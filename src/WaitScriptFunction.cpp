#include "WaitScriptFunction.h"
#include "Script.h"

namespace argosClient {

  void WaitScriptFunction::execute(Script& owner, const std::vector<std::string>& args) {
    float timeToWait = getArgAsFloat(args[0]);

    // ...
  }

}
