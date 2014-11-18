#include "command-parser/command-parser.h"

// Our CommandParser
CommandParser cmd;
int errorcode = 0;

// Handle function for parsing an integer argument
int gInt = 1;
void setInt()
{
    if(!cmd.nextInt(gInt))
    {
        // value was not parsed correctly, handle error here
        errorcode |= -2;
    }
}

// Handle function for parsing an 4 uint8 arguments, using defaults values if none supplied
uint8_t gMulti[4] = {0, 1, 2, 3};
char gMultiOut[64] = {"\0"};
void setMulti()
{
    uint8_t defaults[4] = {0, 0, 0, 0};
    for(uint8_t i=0;i<4;++i)
    {
        if(!cmd.nextInt(gMulti[i]))
        {
            errorcode |= -4; // flag that we needed to use default values
            gMulti[i] = defaults[i];
        }
    }

    snprintf(gMultiOut, 64, "multi %d %d %d %d", gMulti[0], gMulti[1], gMulti[2], gMulti[3]);
}

// Handle function for parsing a floating point argument
double gDbl = 1.0;
void setDbl()
{
    if(!cmd.nextFloat(gDbl))
    {
        // value was not parsed correctly, handle error here
        errorcode |= -8;
    }
}

// Handle function for parsing a string argument
char gStr[64] = {"\0"};
void setStr()
{
    if(char *str = cmd.next())
    {
        strncpy(gStr, str, 64);
    }
    else
    {
        // next() token was NULL, handle error here
        errorcode |= -16;
    }
}

// Simple handle function without arguments
void callFnc()
{
    // Do something fun here
}

// Default handler that catches unparsed tokens, unhandled string can be read from Spark.variable "err"
char gErr[64] = {"\0"};
void defaultHandler(const char *token)
{
    errorcode |= -1;
    strncpy(gErr, token, 64);
}

// Spark.function handler, sends its string to the CommandParser
int parseCallback(String msg)
{
    // reset error handling
    errorcode = 0;
    gErr[0] = '\0';
    
    // do parsing
    cmd.parse(msg);
    
    // return error code
    return errorcode;
}

void setup() 
{
    // Setup commands here, takes a command name and function ptr to "void fnc()"
    cmd.addCommand("int", setInt);
    cmd.addCommand("dbl", setDbl);
    cmd.addCommand("str", setStr);
    cmd.addCommand("multi", setMulti);
    cmd.addCommand("call", callFnc);
    
    // Setup default handler that catches all unregistered commands, "void defaultfnc(const char *token)"
    cmd.setDefaultHandler(defaultHandler);
    
    // Make parsed commands visible in the cloud
    Spark.variable("int", &gInt, INT);
    Spark.variable("double", &gDbl, DOUBLE);
    Spark.variable("string", &gStr, STRING);
    Spark.variable("multi", &gMultiOut, STRING);
    Spark.variable("err", &gErr, STRING);
    
    // Setup the function to handle the commands
    Spark.function("parse", parseCallback);
    
    /*
    Usage:
        Example commands:
            int 1234
            dbl 3.14159
            str hello
            multi 255 0 255 128
            call
            
        Multiple commands can be send separated by ';'
            int 321;dbl 2.71828;multi 4 3 2 1
    */
}

void loop() 
{
    delay(100);
}
