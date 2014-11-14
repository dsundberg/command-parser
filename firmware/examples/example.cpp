#include "command-parser/command-parser.h"

enum class MyErrors {NO_ERROR=0, INVALID_COMMAND=1, P_1_MISSING=2, P_2_MISSING=4, SERIAL_NO_MESSAGE=8 };
CommandParser cmd;
MyErrors err;
int handleCommands(String msg)
{
  err = MyErrors::NO_ERROR;
  cmd.parse(msg);
  return err;
}

int a, b;
void someVars()
{
  if(!cmd.nextInt(a))
  {
    err |= MyErrors::P_1_MISSING;
    return;
  }
  if(!cmd.nextInt(b))
  {
    err |= MyErrors::P_2_MISSING;
    return;
  }
  // Do funky stuff with parameters a and b
}

void printSerial()
{
  char *msg = cmd.next();
  if(msg == nullptr)
  {
    err |= SERIAL_NO_MESSAGE;
    return;
  }

  Serial.println("Yay, got something to print");
  while(msg != nullptr)
  {
    Serial.print("got new word: ");
    Serial.println(msg);
  }
}

void default(const char* msg)
{
  Serial.print("error handling: ");
  Serial.println(msg);
  err |= MyErrors::INVALID_COMMAND;
}

void setup {
  Serial.begin(9600);
  Spark.function("runCommand", handleCommands);
  cmd.addCommand("setAB", someVars);
  cmd.addCommand("serial", printSerial);
}

void loop {
  delay(500);
}
