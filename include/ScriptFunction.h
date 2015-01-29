#ifndef SCRIPTFUNCTION_H
#define SCRIPTFUNCTION_H

#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <iostream>

#include "Log.h"

namespace argosClient {

  /**
   * An interface to be implemented by the scriptable functions
   */
  class ScriptFunction {
  public:
    /**
     * Constructs a new ScriptFunctions
     */
    ScriptFunction(const std::string& name, const std::string& type) : _name(name), _type(type) { }

    /**
     * Destroys the ScriptFunction
     */
    virtual ~ScriptFunction() { }

    /**
     * Execute this ScriptFunctions
     * @param args The list of arguments of this ScriptFunctions
     */
    virtual void execute(const std::vector<std::string>& args, int id) {
      Log::function(_type, args);
      _execute(args, id);
    }

    virtual void _execute(const std::vector<std::string>& args, int id) = 0;

    /**
     * Retrieves the specified property by its key
     * @param key The name of the property to retrieve
     * @return The specified property
     */
    virtual std::string& getProperty(const std::string& key) {
      return _properties[key];
    }

    /**
     * Sets the specified key with the value
     * @param key The name of the property to set
     * @param value The value to set the key
     */
    virtual void setProperty(const std::string& key, const std::string& value) {
      _properties[key] = value;
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

    std::string _name; ///< The name of this scriptable function
    std::string _type; ///> The type of this scriptable function
  };

}

#endif
