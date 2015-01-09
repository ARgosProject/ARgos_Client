#ifndef SCRIPTFUNCTION_H
#define SCRIPTFUNCTION_H

#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <iostream>

namespace argosClient {

  /**
   * An interface to be implemented by the scriptable functions
   */
  class ScriptFunction {
  public:
    /**
     * Constructs a new ScriptFunctions
     */
    ScriptFunction() { }

    /**
     * Destroys the ScriptFunction
     */
    virtual ~ScriptFunction() { }

    /**
     * Execute this ScriptFunctions
     * @param args The list of arguments of this ScriptFunctions
     */
    virtual void execute(const std::vector<std::string>& args) = 0;

    /**
     * Retrieves the specified property by its key
     * @param key The name of the property to retrieve
     * @return The specified property
     */
    virtual std::string& getProperty(const std::string& key) {
      return _properties[key];
    }

  protected:
    /**
     * Parses the given argument and returns it as an integer
     * @param arg The argument to be parsed
     * @return The parsed argument as an integer
     */
    virtual int getArgAsInt(const std::string& arg) const {
      return std::stoi(arg);
    }

    /**
     * Parses the given argument and returns it as a float
     * @param arg The argument to be parsed
     * @return The parsed argument as a float
     */
    virtual float getArgAsFloat(const std::string& arg) const {
      return std::stof(arg);
    }

    /**
     * Parses the given argument and returns it as a boolean
     * @param arg The argument to be parsed
     * @return The parsed argument as a boolean
     */
    virtual bool getArgAsBool(std::string arg) const {
      std::transform(arg.begin(), arg.end(), arg.begin(), ::tolower);
      std::istringstream is(arg);
      bool b;
      is >> std::boolalpha >> b;

      return b;
    }

  protected:
    std::map<std::string, std::string> _properties; ///< An associative list of properties used to hold return values of the ScriptFunction
  };

}

#endif
