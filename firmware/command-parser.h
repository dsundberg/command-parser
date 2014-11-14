#pragma once

#include "application.h"
#include <string.h>

namespace CommandParserConstants
{
  const uint8_t buffer_size = USER_FUNC_ARG_LENGTH;
  const uint8_t command_size = 8;
}

class CommandParser
{
  public:
    CommandParser();
    void addCommand(const char *command, void(*function)());
    void setDefaultHandler(void (*function)(const char *));
    void parse(String msg);
    char *next();
    
    template <typename T>
    bool nextInt(T &val)
    {
      if(auto v = next())
      {
        val = static_cast<T>(atoi(v));
        return true;
      }
      return false;
    }

    template <typename T>
    bool nextFloat(T &val)
    {
      if(auto v = next())
      {
        val = static_cast<T>(atof(v));
        return true;
      }
      return false;
    }
  private:
    struct CommandParserCallback
    {
      char command[CommandParserConstants::command_size + 1];
      void (*function)();
    };
    CommandParserCallback *commandList;
    uint8_t commandCount;

    void (*defaultHandler)(const char *);

    const char *delim, term;
    char *tokenctx;
};
