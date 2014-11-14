#include "command-parser.h"

CommandParser::CommandParser()
  : commandList(nullptr)
  , commandCount(0)
  , defaultHandler(nullptr)
  , delim(" ")
  , term(';')
  , tokenctx(nullptr)
{
}

void CommandParser::addCommand(const char *command, void (*function)()) 
{
  commandList = reinterpret_cast<CommandParserCallback*>(realloc(commandList, (commandCount + 1) * sizeof(CommandParserCallback)));
  strncpy(commandList[commandCount].command, command, CommandParserConstants::command_size);
  commandList[commandCount].function = function;
  ++commandCount;
}

void CommandParser::setDefaultHandler(void (*function)(const char *))
{
  defaultHandler = function;
}

void CommandParser::parse(String msg)
{
  char work[CommandParserConstants::buffer_size];
  uint8_t idx = 0;
  for (uint8_t i = 0; i <= msg.length(); ++i)
  {
    if (i == msg.length() || msg[i] == term) // parse command when we hit command terminator OR buffer has been fully consumed
    {
      work[idx] = '\0'; // terminate work buffer
      idx = 0; // reset work char index   
      char *command = strtok_r(work, delim, &tokenctx);
      if (command != nullptr)
      {
        bool matched = false;
        for (uint8_t j = 0; j < commandCount; ++j)
        {
          if (strncmp(command, commandList[j].command, CommandParserConstants::command_size) == 0)
          {
            (*commandList[j].function)();
            matched = true;
            break;
          }
        }
        if (!matched && (defaultHandler != nullptr))
        {
          (*defaultHandler)(command);
        }
      }
    }
    else 
    {
      work[idx++] = msg[i];
    }
  }
}

char *CommandParser::next()
{
  return strtok_r(nullptr, delim, &tokenctx);
}
